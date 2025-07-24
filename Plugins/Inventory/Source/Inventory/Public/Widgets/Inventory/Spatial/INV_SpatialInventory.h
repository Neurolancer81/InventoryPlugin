// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "INV_SpatialInventory.generated.h"

class UINV_EquippedSlottedItem;
struct FGameplayTag;
class UINV_EquippedGridSlot;
class UINV_ItemDescription;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class UINV_InventoryGrid;
class UINV_HoverItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_SpatialInventory : public UINV_InventoryBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FINV_SlotAvailabilityResult HasRoomForItem(UINV_ItemComponent* ItemComponent) const override;
	virtual void OnItemHovered(UINV_InventoryItem* Item) override;
	virtual void OnItemUnhovered() override;
	virtual bool HasHoverItem() const override;
	virtual UINV_HoverItem* GetHoverItem() const override;
	virtual float GetTileSize() const override;
	
private:

	UPROPERTY()
	TArray<TObjectPtr<UINV_EquippedGridSlot>> EquippedGridSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher>  Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UINV_InventoryGrid> Grid_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UINV_ItemDescription> ItemDescription;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_ItemDescription> EquippedItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UINV_ItemDescription> EquippedItemDescription;

	FTimerHandle DescriptionTimer;
	FTimerHandle EquippedDescriptionTimer;

	UFUNCTION()
	void ShowEquippedItemDescription(UINV_InventoryItem* Item);

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DescriptionTimerDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float EquippedDescriptionTimerDelay = 0.5f;
	
	UINV_ItemDescription* GetItemDescription();
	UINV_ItemDescription* GetEquippedItemDescription();
	
	UFUNCTION()
	void ShowEquippables();	

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UINV_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& Tag);

	UFUNCTION()
	void EquippedSlottedItemClicked(UINV_EquippedSlottedItem* SlottedItem);

	void DisableButton(UButton* Button);
	void SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button);
	void SetItemDescription(UINV_ItemDescription* Description, UCanvasPanel* Canvas) const;
	void SetEquippedItemDescription(UINV_ItemDescription* Description, UINV_ItemDescription* EquippedDescription, UCanvasPanel* Canvas) const;
	bool CanEquipHoverItem(UINV_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const;
	UINV_EquippedGridSlot* FindSlotWithEquippedItem(UINV_InventoryItem* EquippedItem) const;
	void ClearSlotOfItem(UINV_EquippedGridSlot* EquippedGridSlot);
	void RemoveEquippedSlottedItem(UINV_EquippedSlottedItem* EquippedSlottedItem);
	void MakeEquippedSlottedItem(UINV_EquippedSlottedItem* EquippedSlottedItem, UINV_EquippedGridSlot* EquippedGridSlot, UINV_InventoryItem* InventoryItem) const;
	void BroadcastSlotClickedDelegates(UINV_InventoryItem* ItemToEquip, UINV_InventoryItem* ItemToUnequip) const;
	TWeakObjectPtr<UINV_InventoryGrid> ActiveGrid;
	
	
};
