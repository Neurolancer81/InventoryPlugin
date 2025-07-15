// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlot/INV_GridSlot.h"
#include "Items/INV_InventoryItem.h"
#include "Widgets/ItemPopUp/INV_ItemPopUp.h"
#include "Components/Image.h"

void UINV_GridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	GridSlotHovered.Broadcast(TileIndex, InMouseEvent);
}

void UINV_GridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	GridSlotUnhovered.Broadcast(TileIndex, InMouseEvent);
}

FReply UINV_GridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast(TileIndex, InMouseEvent);
	return FReply::Handled();
}

void UINV_GridSlot::SetInventoryItem(UINV_InventoryItem* Item)
{
	InventoryItem = Item; 
}

void UINV_GridSlot::SetItemPopUp(UINV_ItemPopUp* PopUp)
{
	ItemPopUp = PopUp;
	ItemPopUp->SetGridIndex(GetIndex());
	ItemPopUp->OnNativeDestruct.AddUObject(this, &ThisClass::OnItemPopUpDestruct);
}

UINV_ItemPopUp* UINV_GridSlot::GetItemPopUp() const
{
	return ItemPopUp.Get();
}

void UINV_GridSlot::SetUnoccupiedTexture()
{
	GridSlotState = EINV_GridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UINV_GridSlot::SetOccupiedTexture()
{
	GridSlotState = EINV_GridSlotState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void UINV_GridSlot::SetSelectedTexture()
{
	GridSlotState = EINV_GridSlotState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void UINV_GridSlot::SetGrayedOutTexture()
{
	GridSlotState = EINV_GridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}

void UINV_GridSlot::OnItemPopUpDestruct(UUserWidget* Menu)
{
	ItemPopUp.Reset();
}
