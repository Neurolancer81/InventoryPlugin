#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/INV_GridTypes.h"
#include  "StructUtils/InstancedStruct.h"

#include "INV_ItemManifest.generated.h"

class UINV_InventoryItem;
/**
 *This Manifest contains the necessary data for creating an Inventory item
 *
 **/

USTRUCT(BlueprintType)
struct INVENTORY_API FINV_ItemManifest
{
	GENERATED_BODY()

	UINV_InventoryItem* Manifest(UObject* NewOuter);
	EINV_ItemCategory GetItemCategory() const {return ItemCategory;}
	FGameplayTag GetItemType() const {return ItemType;}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EINV_ItemCategory ItemCategory{EINV_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag ItemType;
};