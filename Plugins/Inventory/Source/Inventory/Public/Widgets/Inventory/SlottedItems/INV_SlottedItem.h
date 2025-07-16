// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INV_SlottedItem.generated.h"

class UImage;
class UINV_InventoryItem;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);
/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	/**  Getters and Setters **/
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStackable) {bIsStackable = bStackable;}

	UImage* GetImageIcon() const { return Image_Icon; }
	void SetImageIcon(UImage* Image) { Image_Icon = Image; }

	int32 GetGridIndex() const { return GridIndex; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }

	UINV_InventoryItem* GetInventoryItem() const {return InventoryItem.Get();}
	void SetInventoryItem(UINV_InventoryItem* Item);

	void SetImageBrush(const FSlateBrush& Brush) const;

	void SetStackCount(int32 StackCount);

	/** Delegates **/

	FSlottedItemClicked OnSlottedItemClicked;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 GridIndex;
	FIntPoint GridSize;
	TWeakObjectPtr<UINV_InventoryItem> InventoryItem;
	bool bIsStackable {false};
};
