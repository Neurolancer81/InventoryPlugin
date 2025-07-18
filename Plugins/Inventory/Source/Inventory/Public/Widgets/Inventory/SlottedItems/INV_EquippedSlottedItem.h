// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "INV_SlottedItem.h"
#include "INV_EquippedSlottedItem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, UINV_EquippedSlottedItem*, SlottedItem);
/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_EquippedSlottedItem : public UINV_SlottedItem
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetEquipmentTypeTag(const FGameplayTag& Tag) {EquipmentTypeTag = Tag;}
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }

	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;

private:

	FGameplayTag EquipmentTypeTag;
	
};
