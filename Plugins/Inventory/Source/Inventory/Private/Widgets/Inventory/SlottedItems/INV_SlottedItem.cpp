// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItems/INV_SlottedItem.h"
#include "Items/INV_InventoryItem.h" // This is here because the inventory item in this class is a weak pointer
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Inventory/Public/InventoryManagement/Components/INV_InventoryComponent.h"

FReply UINV_SlottedItem::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, MouseEvent);
	return FReply::Handled();
}

void UINV_SlottedItem::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!InventoryComponent) return;
	UINV_InventoryStatics::ItemHovered(InventoryComponent, InventoryItem.Get());
}

void UINV_SlottedItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (!InventoryComponent) return;
	UINV_InventoryStatics::ItemUnHovered(InventoryComponent);
}

void UINV_SlottedItem::SetInventoryItem(UINV_InventoryItem* Item)
{
	InventoryItem = Item; 
}

void UINV_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UINV_SlottedItem::SetStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(StackCount));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
