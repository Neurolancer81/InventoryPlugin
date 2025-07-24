// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "INV_EquipmentComponent.generated.h"


struct FGameplayTag;
struct FINV_ItemManifest;
struct FINV_EquipmentFragment;
class AINV_EquipActor;
class UINV_InventoryItem;
class UINV_InventoryComponent;
class APlayerController;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UINV_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void SetOwningSkeletalMesh(USkeletalMeshComponent* OwningMesh);
	void SetIsProxy(const bool Proxy) { bIsProxy = Proxy; }

	void InitializeOwner(APlayerController* PlayerController);
	
protected:
	void InitInventoryComponent();
	void InitPlayerController();
	virtual void BeginPlay() override;

private:
	
	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UFUNCTION()
	void OnItemEquipped(UINV_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnequipped(UINV_InventoryItem* UnequippedItem);

	AINV_EquipActor* SpawnEquippedActor(FINV_EquipmentFragment* EquipmentFragment, const FINV_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);

	UPROPERTY()
	TArray<TObjectPtr<AINV_EquipActor>> EquippedActors;

	AINV_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTag);

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	bool bIsProxy = false;
};
