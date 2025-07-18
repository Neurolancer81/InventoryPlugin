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

void UINV_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UINV_SpatialInventory::SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button)
{
	
	if (ActiveGrid.IsValid()) ActiveGrid->HideCursor();
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
