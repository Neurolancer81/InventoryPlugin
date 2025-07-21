// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlot/INV_EquippedGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Fragments/INV_FragmentTags.h"
#include "Items/Fragments/INV_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/INV_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/INV_EquippedSlottedItem.h"

void UINV_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (!IsAvailable()) return;
	UINV_InventoryComponent* InventoryComponent = GetOwningPlayer()->GetComponentByClass<UINV_InventoryComponent>();
	if (!InventoryComponent) return;
	UINV_HoverItem* HoverItem = UINV_InventoryStatics::GetHoverItem(InventoryComponent);
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UINV_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (!IsAvailable()) return;
	UINV_InventoryComponent* InventoryComponent = GetOwningPlayer()->GetComponentByClass<UINV_InventoryComponent>();
	if (!InventoryComponent) return;
	UINV_HoverItem* HoverItem = UINV_InventoryStatics::GetHoverItem(InventoryComponent);
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetUnoccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply UINV_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}

UINV_EquippedSlottedItem* UINV_EquippedGridSlot::OnItemEquipped(UINV_InventoryItem* Item,
	const FGameplayTag& EquipmentTag, float TileSize)
{
	// Check the tag first to see if it is in the right place
	if (!EquipmentTag.MatchesTagExact(EquipmentTypeTag)) return nullptr;
	// Get the Grid Dimensions of the item	
	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridSize = GridFragment->GetGridSize();
	// Calculate the draw size for the equipped slotted item
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding()*2;
	const FVector2D DrawSize = GridSize * IconTileWidth;
	
	// Create the widget for the equipped Slotted item
	EquippedSlottedItem = CreateWidget<UINV_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);
	
	// Set the slotted item's inventory item
	EquippedSlottedItem->SetInventoryItem(Item);
	
	// Set slotted item's equipment type tag
	EquippedSlottedItem->SetEquipmentTypeTag(EquipmentTag);
	
	// Hide the stack count as it is redundant
	EquippedSlottedItem->SetStackCount(0);
	
	// Set the inventory item for this class
	SetInventoryItem(Item);
	
	// Add the right image brush and pass it on to this class
	const FINV_ImageFragment* ImageFragment = GetFragment<FINV_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!ImageFragment) return nullptr;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;

	EquippedSlottedItem->SetImageBrush(Brush);
	
	// Add a slotted item as a child to the widget's overlay
	Overlay_Root->AddChildToOverlay(EquippedSlottedItem);
	FGeometry Overlay_Geometry = Overlay_Root->GetCachedGeometry();
	auto OverlayPosition = Overlay_Geometry.Position;
	auto OverlaySize = Overlay_Geometry.Size;
	
	const float LeftPadding = OverlaySize.X/2.f - DrawSize.X/2.f;
	const float TopPadding =  OverlaySize.Y/2.f - DrawSize.Y/2.f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
	
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));
	
	// return the equipped slotted item widget
	return EquippedSlottedItem;

}

