﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/HoverItem/INV_HoverItem.h"
#include "Items/INV_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UINV_HoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UINV_HoverItem::SetStackCount(const int32 Count)
{
	StackCount = Count;
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UINV_HoverItem::GetItemType() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem->GetItemManifest().GetItemType();
	}

	return FGameplayTag();
}

void UINV_HoverItem::SetIsStackable(bool bStacks)
{
	bIsStackable = bStacks;
	if (!bStacks)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UINV_InventoryItem* UINV_HoverItem::GetInventoryItem() const
{
	return InventoryItem.Get();
}

void UINV_HoverItem::SetInventoryItem(UINV_InventoryItem* Item)
{
	InventoryItem = Item;
}
