// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Public/Widgets/Inventory/Spatial/INV_SpatialInventory.h"

#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Items/INV_InventoryItem.h"
#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"
#include "Widgets/ItemDescription/INV_ItemDescription.h"
#include "Items/Manifest/INV_ItemManifest.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/GridSlot/INV_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/INV_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/INV_EquippedSlottedItem.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"



void UINV_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);

	ShowEquippables();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UINV_EquippedGridSlot* Slot = Cast<UINV_EquippedGridSlot>(Widget); IsValid(Slot))
		{
			EquippedGridSlots.Add(Slot);
			Slot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});
}

void UINV_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;
	SetItemDescription(ItemDescription, CanvasPanel);
	
}

FReply UINV_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

FINV_SlotAvailabilityResult UINV_SpatialInventory::HasRoomForItem(UINV_ItemComponent* ItemComponent) const
{
	switch (UINV_InventoryStatics::GetItemCategoryFromItemComponent(ItemComponent))
	{
		case EINV_ItemCategory::Equippable:
			return Grid_Equippables->HasRoomForItem(ItemComponent);

		case EINV_ItemCategory::Consumable:
			return Grid_Consumables->HasRoomForItem(ItemComponent);

		case EINV_ItemCategory::Craftable:
			return Grid_Craftables->HasRoomForItem(ItemComponent);

		default:
			UE_LOG(LogInventory, Error, TEXT("ItemComponent does not have a valid Item Category"));
			return FINV_SlotAvailabilityResult();
		
	}
}

void UINV_SpatialInventory::OnItemHovered(UINV_InventoryItem* Item)
{
	const FINV_ItemManifest& Manifest = Item->GetItemManifest();
	UINV_ItemDescription* ItemDescriptionWidget = GetItemDescription();
	ItemDescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, ItemDescriptionWidget]()
	{
		Manifest.AssimilateInventoryFragments(ItemDescriptionWidget);
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
	});
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UINV_SpatialInventory::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UINV_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;

	return Super::HasHoverItem();
}

UINV_HoverItem* UINV_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

float UINV_SpatialInventory::GetTileSize() const
{
	return Grid_Consumables->GetTileSize();
}

UINV_ItemDescription* UINV_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UINV_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void UINV_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UINV_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UINV_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UINV_SpatialInventory::EquippedGridSlotClicked(UINV_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& Tag)
{
	// Check to see if we can equip the hover item
	if (!CanEquipHoverItem(EquippedGridSlot, Tag)) return;

	// Get the hover item
	UINV_HoverItem* HoverItem = GetHoverItem();
	
	// Create an equipped Slotted Item and add it to the equipped grid slot
	UINV_InventoryComponent* IC = GetOwningPlayer()->GetComponentByClass<UINV_InventoryComponent>();
	if (!IC) return;
	const float TileSize = UINV_InventoryStatics::GetInventoryWidget(IC)->GetTileSize();
	UINV_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		Tag,
		TileSize);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	
	
	// Clear Hover item
	Grid_Equippables->ClearHoverItem();
	
	// Tell Server we have equipped an item (potentially unequipping an item)
	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	// If we are a client
	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	// If we are a server but not a dedicated server
	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}
	
}

void UINV_SpatialInventory::EquippedSlottedItemClicked(UINV_EquippedSlottedItem* SlottedItem)
{
	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	// Collapse the Item Description
	UINV_InventoryStatics::ItemUnHovered(InventoryComponent);

	UINV_HoverItem* HoverItem = GetHoverItem();
	if (IsValid(HoverItem) && HoverItem->IsStackable()) return;
	// Item to equip
	UINV_InventoryItem* ItemToEquip = HoverItem? HoverItem->GetInventoryItem() : nullptr;	
	
	// Get Item to Unequip
	UINV_InventoryItem* ItemToUnequip = SlottedItem->GetInventoryItem();

	// Get the Equipped Grid Slot holding this item
	UINV_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);
	
	// Clear the equipped grid slot for this item
	ClearSlotOfItem(EquippedGridSlot);
	
	// Remove the equipped slotted item from equipped grid slot
	RemoveEquippedSlottedItem(SlottedItem);
	
	// Assign equipped item as the hover item.
	Grid_Equippables->AssignHoverItem(ItemToUnequip);
	
	// Make a new equipped slotted item if we already had a valid hover item
	MakeEquippedSlottedItem(SlottedItem, EquippedGridSlot, ItemToEquip);

	// Broadcast any delegates
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);	
	
}

void UINV_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UINV_SpatialInventory::SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button)
{
	
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
		ActiveGrid->OnHide();
	}
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) Grid->ShowCursor();
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

void UINV_SpatialInventory::SetItemDescription(UINV_ItemDescription* Description, UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!ItemDescriptionCPS) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UINV_WidgetUtils::GetClampedWidgetPosition(
		UINV_WidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

bool UINV_SpatialInventory::CanEquipHoverItem(UINV_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UINV_HoverItem* HoverItem = GetHoverItem();
	if (!HoverItem) return false;

	UINV_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem() && IsValid(HeldItem) &&
			!HoverItem->IsStackable() &&
				HeldItem->GetItemManifest().GetItemCategory()==EINV_ItemCategory::Equippable &&
					HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
	
}

UINV_EquippedGridSlot* UINV_SpatialInventory::FindSlotWithEquippedItem(UINV_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const UINV_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UINV_SpatialInventory::ClearSlotOfItem(UINV_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
		
	}
}

void UINV_SpatialInventory::RemoveEquippedSlottedItem(UINV_EquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);		
	}
	EquippedSlottedItem->RemoveFromParent();
}

void UINV_SpatialInventory::MakeEquippedSlottedItem(UINV_EquippedSlottedItem* EquippedSlottedItem,
	UINV_EquippedGridSlot* EquippedGridSlot, UINV_InventoryItem* InventoryItem) const
{
	if (!IsValid(EquippedGridSlot)) return;

	UINV_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(
		InventoryItem,
		EquippedSlottedItem->GetEquipmentTypeTag(),
		UINV_InventoryStatics::GetInventoryWidget(UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer()))->GetTileSize()
		);

	if (SlottedItem != nullptr)
	{
		SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}	
	
	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UINV_SpatialInventory::BroadcastSlotClickedDelegates(UINV_InventoryItem* ItemToEquip,
	UINV_InventoryItem* ItemToUnequip) const
{
	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(InventoryComponent);

	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemUnEquipped.Broadcast(ItemToUnequip);
	}
}
