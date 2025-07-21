// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "INV_GridSlot.h"
#include "INV_EquippedGridSlot.generated.h"

class UOverlay;
class UINV_EquippedSlottedItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UINV_EquippedGridSlot*, GridSlot,
                                             const FGameplayTag&, Tag);

class UImage;

/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_EquippedGridSlot : public UINV_GridSlot
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UINV_EquippedSlottedItem* OnItemEquipped(UINV_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize);
	void SetEquippedSlottedItem(UINV_EquippedSlottedItem* Item) {EquippedSlottedItem = Item;}
	FEquippedGridSlotClicked EquippedGridSlotClicked;

private:

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(Categories="GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_GrayedOutIcon;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_EquippedSlottedItem> EquippedSlottedItemClass;

	UPROPERTY()
	TObjectPtr<UINV_EquippedSlottedItem> EquippedSlottedItem;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
};
