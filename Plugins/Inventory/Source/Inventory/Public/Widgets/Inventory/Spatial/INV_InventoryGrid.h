// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/INV_GridTypes.h"

#include "INV_InventoryGrid.generated.h"

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

	UFUNCTION()
	void AddItem(UINV_InventoryItem* Item);

private:

	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;

	void ConstructGrid();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	EINV_ItemCategory ItemCategory;

	UPROPERTY()
	TArray<TObjectPtr<UINV_GridSlot>> GridSlots;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	bool MatchesCategory(const UINV_InventoryItem* Item) const;
};
