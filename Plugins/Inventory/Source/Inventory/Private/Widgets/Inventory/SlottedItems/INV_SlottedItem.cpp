// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItems/INV_SlottedItem.h"
#include "Items/INV_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
