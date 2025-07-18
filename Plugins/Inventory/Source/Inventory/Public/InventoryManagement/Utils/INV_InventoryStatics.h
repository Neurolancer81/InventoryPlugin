﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Utils/INV_WidgetUtils.h"
#include "INV_InventoryStatics.generated.h"

class UINV_HoverItem;
class UINV_InventoryItem;
class UINV_ItemComponent;
enum class EINV_ItemCategory : uint8;
class UINV_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UINV_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EINV_ItemCategory GetItemCategoryFromItemComponent(UINV_ItemComponent* ItemComponent);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemHovered(UINV_InventoryComponent* InventoryComponent, UINV_InventoryItem* Item);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemUnHovered(UINV_InventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UINV_HoverItem* GetHoverItem(UINV_InventoryComponent* InventoryComponent);
};

template<typename T, typename FuncT>
void UINV_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for (int32 j = 0; j < Range2D.Y; ++j)
	{
		for (int32 i = 0; i < Range2D.X; ++i)
		{
			const FIntPoint Coordinates = UINV_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(i, j);
			const int32 TileIndex = UINV_WidgetUtils::GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}