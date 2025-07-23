// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/ProxyMesh/INV_ProxyMesh.h"

#include "IEditableSkeleton.h"
#include "EquipmentManagement/Components/INV_EquipmentComponent.h"


// Sets default values
AINV_ProxyMesh::AINV_ProxyMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	ProxyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Proxy Mesh");
	ProxyMesh->SetupAttachment(RootComponent);

	EquipmentComponent = CreateDefaultSubobject<UINV_EquipmentComponent>("Equipment");
	EquipmentComponent->SetOwningSkeletalMesh(ProxyMesh);
	EquipmentComponent->SetIsProxy(true);
}


void AINV_ProxyMesh::BeginPlay()
{
	Super::BeginPlay();
	
}


