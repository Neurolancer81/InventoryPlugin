#pragma once

#include "INV_GridTypes.generated.h"

class UINV_InventoryItem;

UENUM(BlueprintType)
enum class EINV_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

// This structure is per slot. It tells us what's in it.
USTRUCT()
struct FINV_SlotAvailability
{
	GENERATED_BODY()

	FINV_SlotAvailability() {}
	FINV_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	int32 Index{INDEX_NONE};
	int32 AmountToFill{0};
	bool bItemAtIndex{false};
};



USTRUCT()
struct FINV_SlotAvailabilityResult
{
	GENERATED_BODY()

	FINV_SlotAvailabilityResult() {}

	TWeakObjectPtr<UINV_InventoryItem> Item;
	int32 TotalRoomToFill{0};
	int32 Remainder{0};
	bool bStackable = false;
	TArray<FINV_SlotAvailability> SlotAvailabilities;
	
};

//
UENUM(BlueprintType)
enum class EINV_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FINV_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinates{0,0};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EINV_TileQuadrant TileQuadrant{EINV_TileQuadrant::None};
	
};

inline bool operator==(const FINV_TileParameters& A, const FINV_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates &&
		A.TileIndex == B.TileIndex &&
		A.TileQuadrant == B.TileQuadrant;
}

USTRUCT()
struct FINV_SpaceQueryResult
{
	GENERATED_BODY()

	// True if the space queried has no item in it
	bool bHasSpace{false};

	// If there is a swap-able single item
	TWeakObjectPtr<UINV_InventoryItem> ValidItem = nullptr;

	// If item is valid, this is its upper left index
	int32 UpperLeftIndex{INDEX_NONE};

	
};