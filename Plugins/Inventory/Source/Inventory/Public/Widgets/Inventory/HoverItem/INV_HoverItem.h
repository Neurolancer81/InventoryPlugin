// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "INV_HoverItem.generated.h"


class UINV_InventoryItem;
class UTextBlock;
class UImage;
/**
 * This is the item that appears and follows the mouse when an inventory item on the grid was clicked.
 */
UCLASS()
class INVENTORY_API UINV_HoverItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetImageBrush(const FSlateBrush& Brush) const;
	void SetStackCount(const int32 Count) const;

	FGameplayTag GetItemType() const;

	int32 GetStackCount() const {return StackCount;}
	bool IsStackable() const {return bIsStackable;}
	void SetIsStackable(bool bStacks);

	int32 GetPreviousGridIndex() const {return PreviousGridIndex;}
	void SetPreviousGridIndex(int32 Index) {PreviousGridIndex = Index;}

	FIntPoint GetGridDimensions() const {return GridDimensions;}
	void SetGridDimensions(const FIntPoint& Dimensions) {GridDimensions = Dimensions;}

	UINV_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UINV_InventoryItem* Item);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
};
