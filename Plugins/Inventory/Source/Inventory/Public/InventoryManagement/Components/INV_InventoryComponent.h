// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/INV_FastArray.h"
#include "INV_InventoryComponent.generated.h"


struct FINV_SlotAvailabilityResult;
class UINV_ItemComponent;
class UINV_InventoryItem;
class UINV_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, UINV_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FINV_SlotAvailabilityResult&, Result);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UINV_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UINV_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UINV_ItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UINV_ItemComponent* ItemComponent, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UINV_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UINV_InventoryItem* Item, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UINV_InventoryItem* Item);

	void ToggleInventoryMenu();
	void AddReplicatedSubObj(UObject* SubObject);
	void SpawnDroppedItem(UINV_InventoryItem* Item, int32 StackCount);

	// Broadcast delegate when inventory item is added or removed
	FInventoryItemChange OnItemAdded;
	FInventoryItemChange OnItemRemoved;
	FNoRoomInInventory OnNoRoomInInventory;
	FStackChange OnStackChange;
	
protected:
	
	virtual void BeginPlay() override;

	

private:
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	void ConstructInventory();

	UPROPERTY(Replicated)
	FINV_InventoryFastArray InventoryList;

	UPROPERTY()
	TObjectPtr<UINV_InventoryBase> InventoryMenu;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UINV_InventoryBase> InventoryMenuClass;

	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMin = 10.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float RelativeSpawnLocation = -70.f;
};
