// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Utils/INV_InventoryStatics.h"

#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Types/INV_GridTypes.h"

UINV_InventoryComponent* UINV_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	return PlayerController->FindComponentByClass<UINV_InventoryComponent>();
}

EINV_ItemCategory UINV_InventoryStatics::GetItemCategoryFromItemComponent(UINV_ItemComponent* ItemComponent)
{
	if (!IsValid(ItemComponent)) return EINV_ItemCategory::None;
	return ItemComponent->GetItemManifest().GetItemCategory();
}
