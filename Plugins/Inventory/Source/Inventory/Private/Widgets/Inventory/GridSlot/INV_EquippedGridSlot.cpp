// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlot/INV_EquippedGridSlot.h"

#include "Components/Image.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Widgets/Inventory/HoverItem/INV_HoverItem.h"

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
	// Get the Grid Dimensions of the item
	// Calculate the draw size for the equipped slotted item
	// Create the widget for the equipped Slotted item
	// Set the slotted item's inventory item
	// Set slotted item's equipment type tag
	// Hide the stack count as it is redundant
	// Set the inventory item for this class
	// Add the right image brush and pass it on to this class
	


	return nullptr;
}
