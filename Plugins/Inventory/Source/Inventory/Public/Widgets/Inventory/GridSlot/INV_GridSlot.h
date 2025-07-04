// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualizeTexture.h"
#include "Blueprint/UserWidget.h"
#include "INV_GridSlot.generated.h"

class UINV_InventoryItem;
class UImage;
/**
 * 
 */
UENUM(BlueprintType)
enum class EINV_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};
UCLASS()
class INVENTORY_API UINV_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTileIndex(int32 i) { TileIndex = i; }
	int32 GetTileIndex() const { return TileIndex; }

	EINV_GridSlotState GetGridSlotState() const { return GridSlotState; }
	void SetGridSlotState(EINV_GridSlotState State) { GridSlotState = State; }

	TWeakObjectPtr<UINV_InventoryItem> GetInventoryItem() const{ return InventoryItem; }
	void SetInventoryItem(UINV_InventoryItem* Item) {InventoryItem = Item; }

	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

	int32 GetIndex() const { return TileIndex; }
	void SetIndex(int32 Index) { TileIndex = Index; }

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }

	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool b) { bAvailable = b; }
	
	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

private:

	int32 TileIndex;
	int32 StackCount;
	int32 UpperLeftIndex{INDEX_NONE};
	bool bAvailable;

	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	EINV_GridSlotState GridSlotState;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;


};
