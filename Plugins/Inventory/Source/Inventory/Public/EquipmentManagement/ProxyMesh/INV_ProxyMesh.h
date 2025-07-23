// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INV_ProxyMesh.generated.h"

class UINV_EquipmentComponent;

UCLASS()
class INVENTORY_API AINV_ProxyMesh : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AINV_ProxyMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UINV_EquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ProxyMesh;
	

};
