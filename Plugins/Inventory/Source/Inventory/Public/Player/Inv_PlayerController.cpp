// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

