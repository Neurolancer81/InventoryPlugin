// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/INV_ItemManifest.h"
#include "INV_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UINV_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UINV_ItemComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FINV_ItemManifest GetItemManifest() const { return ItemManifest; }

	FString GetPickupMessage() const { return PickupMessage; }

	void PickedUp();

	

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OnPickedUp();


private:
	UPROPERTY(Replicated, EditAnywhere, Category="Inventory")
	FINV_ItemManifest ItemManifest;
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickupMessage;
};
