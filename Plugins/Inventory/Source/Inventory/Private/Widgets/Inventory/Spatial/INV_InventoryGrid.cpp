// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/INV_InventoryGrid.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Items/Fragments/INV_FragmentTags.h"
#include "Widgets/Inventory/GridSlot/INV_GridSlot.h"
#include "Widgets/Utils/INV_WidgetUtils.h"
#include "Items/Manifest/INV_ItemManifest.h"
#include "Widgets/Inventory/SlottedItems/INV_SlottedItem.h"

void UINV_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const FINV_ItemManifest& Manifest)
{
	FINV_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 10;
	Result.bStackable = true;

	FINV_SlotAvailability SlotAvailability;	
	SlotAvailability.Index = 0;
	SlotAvailability.AmountToFill = 2;

	FINV_SlotAvailability SlotAvailability2;
	SlotAvailability2.Index = 1;
	SlotAvailability2.AmountToFill = 5;
	

	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability));
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability2));
	
	return Result;
}

FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());	
}



FINV_SlotAvailabilityResult UINV_InventoryGrid::HasRoomForItem(const UINV_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}


void UINV_InventoryGrid::AddItem(UINV_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FINV_SlotAvailabilityResult Result = HasRoomForItem(Item);
	AddItemToIndices(Result, Item);	
}

void UINV_InventoryGrid::AddItemToIndices(const FINV_SlotAvailabilityResult& Result, UINV_InventoryItem* NewItem)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlot(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
	
}

FVector2D UINV_InventoryGrid::GetDrawSize(const FINV_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize -GridFragment->GetGridPadding()*2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UINV_InventoryGrid::SetSlottedItemImage(const UINV_SlottedItem* SlottedItem, const FINV_GridFragment* GridFragment,
	const FINV_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;	
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

void UINV_InventoryGrid::AddItemAtIndex(UINV_InventoryItem* Item, const int32 Index, const bool bStackable,
	const int32 StackAmount)
{
	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(Item, FragmentTags::GridFragment);
	const FINV_ImageFragment* ImageFragment = GetFragment<FINV_ImageFragment>(Item, FragmentTags::IconFragment);

	if (!GridFragment || !ImageFragment) return;

	UINV_SlottedItem* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);
	AddSlottedItemToCanvas(SlottedItem, GridFragment, Index);

	SlottedItems.Add(Index, SlottedItem);
}

UINV_SlottedItem* UINV_InventoryGrid::CreateSlottedItem(UINV_InventoryItem* Item, const bool bStackable,
	const int32 StackAmount, const FINV_GridFragment* GridFragment, const FINV_ImageFragment* ImageFragment,
	const int32 Index) const
{
	UINV_SlottedItem* SlottedItem = CreateWidget<UINV_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->SetStackCount(StackUpdateAmount);

	return SlottedItem;
}

void UINV_InventoryGrid::AddSlottedItemToCanvas(UINV_SlottedItem* SlottedItem, const FINV_GridFragment* GridFragment,
	const int32 Index) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasPanelSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPosition = UINV_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPositionWithPadding = DrawPosition + FVector2D(GridFragment->GetGridPadding());
	CanvasPanelSlot->SetPosition(DrawPositionWithPadding);
	
}

void UINV_InventoryGrid::UpdateGridSlot(UINV_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));
	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FINV_GridFragment* GridFragment = GetFragment<FINV_GridFragment>(NewItem, FragmentTags::GridFragment);
	
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UINV_InventoryStatics::ForEach2D(GridSlots, Index,Dimensions, Columns,
		[&](UINV_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(NewItem);
			GridSlot->SetUpperLeftIndex(Index);
			GridSlot->SetOccupiedTexture();
			GridSlot->SetAvailable(false);
		});
	
}


void UINV_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			FIntPoint TilePosition = FIntPoint(j, i);
			UINV_GridSlot* GridSlot = CreateWidget<UINV_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			int32 index = UINV_WidgetUtils::GetIndexFromPosition(TilePosition, Columns);
			GridSlot->SetTileIndex(index);

			UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			CanvasPanelSlot->SetSize(FVector2D(TileSize));
			CanvasPanelSlot->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
			
}



bool UINV_InventoryGrid::MatchesCategory(const UINV_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}
