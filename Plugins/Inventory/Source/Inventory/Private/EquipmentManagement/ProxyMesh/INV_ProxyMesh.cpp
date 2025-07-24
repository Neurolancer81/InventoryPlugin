// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagement/ProxyMesh/INV_ProxyMesh.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "EquipmentManagement/Components/INV_EquipmentComponent.h"
#include "GameFramework/Character.h"
#include "InventoryManagement/Utils/INV_InventoryStatics.h"


AINV_ProxyMesh::AINV_ProxyMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);

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
	DelayedInitializeOwner();
}

void AINV_ProxyMesh::DelayedInitializeOwner()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		DelayedInitialization();
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PC))
	{
		DelayedInitialization();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(PC->GetPawn());
	if (!IsValid(Character))
	{
		DelayedInitialization();
		return;
	}

	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (!IsValid(CharacterMesh))
	{
		DelayedInitialization();
		return;
	}

	UINV_InventoryComponent* InventoryComponent = UINV_InventoryStatics::GetInventoryComponent(PC);
	if (!InventoryComponent)
	{
		DelayedInitialization();
		return;
	}

	SourceMesh = CharacterMesh;
	ProxyMesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	ProxyMesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());
	
	EquipmentComponent->InitializeOwner(PC);
}

void AINV_ProxyMesh::DelayedInitialization()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::DelayedInitializeOwner);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
	
}


