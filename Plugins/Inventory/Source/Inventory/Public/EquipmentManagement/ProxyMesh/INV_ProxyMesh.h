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
	AINV_ProxyMesh();
	USkeletalMeshComponent* GetMesh() const { return ProxyMesh; }

protected:
	
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory")
	TObjectPtr<UINV_EquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	TObjectPtr<USkeletalMeshComponent> ProxyMesh;

	FTimerHandle TimerForNextTick;
	void DelayedInitializeOwner();
	void DelayedInitialization();
	

};
