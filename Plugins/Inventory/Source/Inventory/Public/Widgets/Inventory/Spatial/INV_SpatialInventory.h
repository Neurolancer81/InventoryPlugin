// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "INV_SpatialInventory.generated.h"

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

	FTimerHandle DescriptionTimer;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DescriptionTimerDelay = 0.5f;

	UINV_ItemDescription* GetItemDescription();
	
	UFUNCTION()
	void ShowEquippables();	

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UINV_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& Tag);

	void DisableButton(UButton* Button);
	void SetActiveGrid(UINV_InventoryGrid* Grid, UButton* Button);
	void SetItemDescription(UINV_ItemDescription* Description, UCanvasPanel* Canvas) const;
	bool CanEquipHoverItem(UINV_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const;

	TWeakObjectPtr<UINV_InventoryGrid> ActiveGrid;
	
	
};
