#include "Items/Manifest/INV_ItemManifest.h"

#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"

UINV_InventoryItem* FINV_ItemManifest::Manifest(UObject* NewOuter)
{
	UINV_InventoryItem* Item = NewObject<UINV_InventoryItem>(NewOuter, UINV_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	return Item;
}

void FINV_ItemManifest::SpawnPickUpActor(const UObject* WorldContextObject, const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	if (!IsValid(WorldContextObject) || !PickUpActorClass) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickUpActorClass, SpawnLocation, SpawnRotation);
	if (!SpawnedActor) return;

	// Set Item Manifest, Item Category, Item Type etc.
	UINV_ItemComponent* ItemComponent = SpawnedActor->FindComponentByClass<UINV_ItemComponent>();
	check(ItemComponent);

	ItemComponent->InitItemManifest(*this);
}
