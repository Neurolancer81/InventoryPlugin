// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/INV_ItemManifest.h"

#include "INV_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override {return true;}

	void SetItemManifest(const FINV_ItemManifest& Manifest);
	const FINV_ItemManifest& GetItemManifest() const {return ItemManifest.Get<FINV_ItemManifest>();}
	FINV_ItemManifest& GetItemManifestMutable()  {return ItemManifest.GetMutable<FINV_ItemManifest>();}

private:

	UPROPERTY(VisibleAnywhere, Category="Inventory", meta=(BaseStruct = "/Script/Inventory.INV_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
	
};

template <typename FragmentType>
const FragmentType* GetFragment(const UINV_InventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;

	const FINV_ItemManifest& ItemManifest = Item->GetItemManifest();
	return ItemManifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
	
}