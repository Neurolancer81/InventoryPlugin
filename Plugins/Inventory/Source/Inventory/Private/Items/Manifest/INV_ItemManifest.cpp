#include "Items/Manifest/INV_ItemManifest.h"

#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Widgets/Composite/INV_CompositeBase.h"

UINV_InventoryItem* FINV_ItemManifest::Manifest(UObject* NewOuter)
{
	UINV_InventoryItem* Item = NewObject<UINV_InventoryItem>(NewOuter, UINV_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	for (auto& Fragment: Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}
	ClearFragments();

	return Item;
}

void FINV_ItemManifest::AssimilateInventoryFragments(UINV_CompositeBase* Composite) const
{
	const auto& ItemFragments = GetAllFragmentsOfType<FINV_InventoryItemFragment>();
	for (const auto* Fragment : ItemFragments)
	{
		Composite->ApplyFunction([Fragment](UINV_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
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

void FINV_ItemManifest::ClearFragments()
{
	for (auto& Fragment: Fragments)
	{
		Fragment.Reset();
	}
	Fragments.Empty();
}
