
#include "EquipmentManagement/Components/INV_EquipmentComponent.h"

#include "EquipmentManagement/EquipActor/INV_EquipActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"
#include "Items/INV_InventoryItem.h"


void UINV_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	InitPlayerController();
}

void UINV_EquipmentComponent::InitPlayerController()
{	
	if (OwningPlayerController = Cast<APlayerController>(GetOwner());OwningPlayerController.IsValid())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()))
		{			
			OnPossessedPawnChanged(nullptr, OwnerCharacter);
		}
		else
		{
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UINV_EquipmentComponent::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}
	InitInventoryComponent();
}

void UINV_EquipmentComponent::SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh)
{
	OwningSkeletalMesh = OwningMesh;
}

void UINV_EquipmentComponent::InitializeOwner(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OwningPlayerController = PlayerController;
	}
	InitInventoryComponent();
}

void UINV_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid())
	{		
		return;
	}

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::OnItemEquipped);
	}

	if (!InventoryComponent->OnItemUnEquipped.IsAlreadyBound(this, &ThisClass::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnEquipped.AddDynamic(this, &ThisClass::OnItemUnequipped);
	}
}



void UINV_EquipmentComponent::OnItemEquipped(UINV_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;
	if (!InventoryComponent.IsValid()) return;
	if (!OwningSkeletalMesh.IsValid()) return;

	FINV_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FINV_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FINV_EquipmentFragment>();
	if (!EquipmentFragment) return;

	if (!bIsProxy)
	{
		EquipmentFragment->OnEquip(InventoryComponent.Get());	
	}
		
	AINV_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());
	
	EquippedActors.Add(SpawnedEquipActor);
}

void UINV_EquipmentComponent::OnItemUnequipped(UINV_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;
	if (!InventoryComponent.IsValid()) return;
	
	FINV_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FINV_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FINV_EquipmentFragment>();
	if (!EquipmentFragment) return;
	if (!bIsProxy)
	{
		EquipmentFragment->OnUnEquip(InventoryComponent.Get());
	}

	RemoveEquippedActor(EquipmentFragment->GetEquipmentType());
	
}

AINV_EquipActor* UINV_EquipmentComponent::SpawnEquippedActor(FINV_EquipmentFragment* EquipmentFragment,
	const FINV_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	AINV_EquipActor* SpawnedEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquipActor->SetOwner(GetOwner());

	EquipmentFragment->SetEquippedActor(SpawnedEquipActor);
	return SpawnedEquipActor;
}

AINV_EquipActor* UINV_EquipmentComponent::FindEquippedActor(const FGameplayTag& EquipmentTag)
{
	const auto FoundActor = EquippedActors.FindByPredicate([&EquipmentTag](const AINV_EquipActor* Actor)
	{
		return Actor->GetEquipmentType().MatchesTagExact(EquipmentTag);
	});

	return FoundActor ? *FoundActor : nullptr;
}

void UINV_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentTag)
{
	if (AINV_EquipActor* EquippedActor = FindEquippedActor(EquipmentTag); IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}


