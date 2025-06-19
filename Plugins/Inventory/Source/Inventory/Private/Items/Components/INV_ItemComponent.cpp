// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/INV_ItemComponent.h"



UINV_ItemComponent::UINV_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PickupMessage = FString("E - Pick Up!");
}
