﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/INV_ItemComponent.h"

#include "Net/UnrealNetwork.h"


UINV_ItemComponent::UINV_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PickupMessage = FString("E - Pick Up!");
}

void UINV_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UINV_ItemComponent::PickedUp()
{
	OnPickedUp();
	GetOwner()->Destroy();
}
