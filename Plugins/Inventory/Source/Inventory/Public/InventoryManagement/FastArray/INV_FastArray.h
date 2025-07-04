#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "INV_FastArray.generated.h"

struct FGameplayTag;
class UINV_ItemComponent;
class UINV_InventoryComponent;
class UINV_InventoryItem;

/** A Struct holding a single inventory item which will be an element of the fast array **/
USTRUCT(BlueprintType)
struct FINV_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FINV_InventoryEntry() {}

private:
	friend struct FINV_InventoryFastArray;
	friend UINV_InventoryComponent;

	UPROPERTY()
	TObjectPtr<UINV_InventoryItem> Item = nullptr;
};


/** A FastArray of inventory items **/
USTRUCT(BlueprintType)
struct FINV_InventoryFastArray: public FFastArraySerializer
{
	GENERATED_BODY()

	FINV_InventoryFastArray() : OwnerComponent(nullptr) {}
	FINV_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}

	// A helper function to extract an array of pointers to all the inventory items
	TArray<TObjectPtr<UINV_InventoryItem>> GetAllItems() const;

	
	
	/** FFastArraySerializer Contract start**/
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	/** FFastArraySerializer Contract end**/

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaSerializeInfo)
	{
		return FastArrayDeltaSerialize<FINV_InventoryEntry, FINV_InventoryFastArray>(
			Entries, DeltaSerializeInfo, *this);
	}

	// Helper functions to add and remove items from the fast array
	UINV_InventoryItem* AddEntry(UINV_ItemComponent* ItemComponent);
	UINV_InventoryItem* AddEntry(UINV_InventoryItem* Item);	
	void RemoveEntry(UINV_InventoryItem* Item);

	UINV_InventoryItem* FindFirstItemByType(const FGameplayTag& ItemType);
	

private:
	friend UINV_InventoryComponent;
	
	// List of items which will get replicated.
	UPROPERTY()
	TArray<FINV_InventoryEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

// Using type traits to tell the engine to delat serialize these structs

template<>
struct TStructOpsTypeTraits<FINV_InventoryFastArray>: public TStructOpsTypeTraitsBase2<FINV_InventoryFastArray>
{
	enum {WithNetDelatSerializer = true};
};

