// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"

struct FINV_ImageFragment;
struct FINV_GridFragment;
class UINV_SlottedItem;
struct FINV_ItemManifest;
class UINV_ItemComponent;
class UINV_InventoryComponent;
class UCanvasPanel;
class UINV_GridSlot;
class UINV_InventoryItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	EINV_ItemCategory GetItemCategory() const {return ItemCategory;}

	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_ItemComponent* ItemComponent);
	FINV_SlotAvailabilityResult HasRoomForItem(const FINV_ItemManifest& Manifest);

	void AddItemToIndices(const FINV_SlotAvailabilityResult& Result, UINV_InventoryItem* NewItem);
	

	UFUNCTION()
	void AddItem(UINV_InventoryItem* Item);

private:

	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;

	void ConstructGrid();
	bool MatchesCategory(const UINV_InventoryItem* Item) const;
	FINV_SlotAvailabilityResult HasRoomForItem(const UINV_InventoryItem* Item);
	FVector2D GetDrawSize(const FINV_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UINV_SlottedItem* SlottedItem, const FINV_GridFragment* GridFragment,
		const FINV_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(UINV_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	UINV_SlottedItem* CreateSlottedItem(UINV_InventoryItem* Item,
		const bool bStackable,
		const int32 StackAmount,
		const FINV_GridFragment* GridFragment,
		const FINV_ImageFragment* ImageFragment,
		const int32 Index) const;
	void AddSlottedItemToCanvas(UINV_SlottedItem* SlottedItem, const FINV_GridFragment* GridFragment, const int32 Index) const;
	void UpdateGridSlot(UINV_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	EINV_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UINV_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UINV_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UINV_SlottedItem>> SlottedItems;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	
};
