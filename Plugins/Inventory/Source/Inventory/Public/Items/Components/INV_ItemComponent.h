// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "INV_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UINV_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UINV_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }

protected:


private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickupMessage;
};
