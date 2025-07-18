﻿#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/Fragments/INV_ItemFragment.h"
#include "Types/INV_GridTypes.h"
#include  "StructUtils/InstancedStruct.h"

#include "INV_ItemManifest.generated.h"

class UINV_CompositeBase;
class UINV_InventoryItem;
struct FINV_ItemFragment;

/**
 *This Manifest contains the necessary data for creating an Inventory item
 *
 **/

USTRUCT(BlueprintType)
struct INVENTORY_API FINV_ItemManifest
{
	GENERATED_BODY()
	
	TArray<TInstancedStruct<FINV_ItemFragment>>& GetFragmentsMutable() { return Fragments; }
	UINV_InventoryItem* Manifest(UObject* NewOuter);
	EINV_ItemCategory GetItemCategory() const {return ItemCategory;}
	FGameplayTag GetItemType() const {return ItemType;}
	void AssimilateInventoryFragments(UINV_CompositeBase* Composite) const;

	template<typename T> requires std::derived_from<T, FINV_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FINV_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FINV_ItemFragment>
	T* GetFragmentOfTypeMutable();

	template<typename T> requires std::derived_from<T, FINV_ItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;

	void SpawnPickUpActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);

private:

	UPROPERTY(EditAnywhere, Category="Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FINV_ItemFragment>> Fragments;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EINV_ItemCategory ItemCategory{EINV_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(Categories="GameItems"))
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> PickUpActorClass;

	void ClearFragments();
};

template<typename T> requires std::derived_from<T, FINV_ItemFragment>
const T* FINV_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FINV_ItemFragment>& Fragment : Fragments)
	{
		if(const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;			
			return FragmentPtr;
		}
	}


	return nullptr;
}

template <typename T> requires std::derived_from<T, FINV_ItemFragment>
const T* FINV_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FINV_ItemFragment>& Fragment : Fragments)
	{
		if(const T* FragmentPtr = Fragment.GetPtr<T>())
		{					
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FINV_ItemFragment>
T* FINV_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FINV_ItemFragment>& Fragment : Fragments)
	{
		if(T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{					
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FINV_ItemFragment>
TArray<const T*> FINV_ItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> ItemFragments;
	for (const TInstancedStruct<FINV_ItemFragment>& Fragment : Fragments)
	{
		if(const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			ItemFragments.Add(FragmentPtr);
		}
	}
	return ItemFragments;
}
