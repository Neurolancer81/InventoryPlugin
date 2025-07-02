#include "InventoryManagement/FastArray/INV_FastArray.h"

#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/INV_InventoryItem.h"
#include "Items/Components/INV_ItemComponent.h"

TArray<TObjectPtr<UINV_InventoryItem>> FINV_InventoryFastArray::GetAllItems() const
{
	TArray<TObjectPtr<UINV_InventoryItem>> Results;
	Results.Reserve(Entries.Num());

	for (const auto& Entry : Entries)
	{
		if (!IsValid(Entry.Item)) continue;
		Results.Add(Entry.Item);
	}

	return Results;
}

void FINV_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	// We can do anything before an item is removed. So, the best way to do that is to broadcast a delegate.
	// We will do that from the Inventory Component

	UINV_InventoryComponent* InventoryComponent = Cast<UINV_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : RemovedIndices)
	{
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);
	}
	
}

void FINV_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	// We can do anything after an item is added. So, the best way to do that is to broadcast a delegate.
	// We will do that from the Inventory Component

	UINV_InventoryComponent* InventoryComponent = Cast<UINV_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);
	}
	
}

UINV_InventoryItem* FINV_InventoryFastArray::AddEntry(UINV_ItemComponent* ItemComponent)
{
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	UINV_InventoryComponent* InventoryComponent = Cast<UINV_InventoryComponent>(OwnerComponent);

	if (!IsValid(InventoryComponent)) return nullptr;

	FINV_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);

	InventoryComponent->AddReplicatedSubObject(NewEntry.Item);
	MarkItemDirty(NewEntry);
	return NewEntry.Item;
	
}

UINV_InventoryItem* FINV_InventoryFastArray::AddEntry(UINV_InventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FINV_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);
	return NewEntry.Item;
}

void FINV_InventoryFastArray::RemoveEntry(UINV_InventoryItem* Item)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FINV_InventoryEntry& Entry = *EntryIt;
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
