// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "INV_EquipActor.generated.h"

UCLASS()
class INVENTORY_API AINV_EquipActor : public AActor
{
	GENERATED_BODY()

public:
	
	AINV_EquipActor();
		
	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(const FGameplayTag NewEquipmentType) {EquipmentType = NewEquipmentType; }
	
private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FGameplayTag EquipmentType;

};
