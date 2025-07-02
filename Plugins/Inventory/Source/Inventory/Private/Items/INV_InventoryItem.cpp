// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/INV_InventoryItem.h"

#include "Net/UnrealNetwork.h"

void UINV_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UINV_InventoryItem::SetItemManifest(const FINV_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FINV_ItemManifest>(Manifest);
}
