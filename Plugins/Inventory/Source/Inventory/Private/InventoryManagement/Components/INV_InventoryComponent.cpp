// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/INV_InventoryComponent.h"

#include "Widgets/Inventory/InventoryBase/INV_InventoryBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UINV_InventoryComponent::UINV_InventoryComponent() : InventoryList(this)
{	
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UINV_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UINV_InventoryComponent::TryAddItem(UINV_ItemComponent* ItemComponent)
{
	FINV_SlotAvailabilityResult Result = InventoryMenu->HasRoomForItem(ItemComponent);

	if (Result.TotalRoomToFill == 0)
	{
		OnNoRoomInInventory.Broadcast();
		return;
	}

	if (Result.Item.IsValid() && Result.bStackable)
	{
		// Add stacks to an item which already exists
		// Update stack count
		Server_AddStacksToItem_Implementation(ItemComponent, Result.TotalRoomToFill, Result.Remainder);
	}

	else if (Result.TotalRoomToFill > 0)
	{
		// Item types does not yet exist
		// Create a new one and update all pertinent slots
		Server_AddNewItem_Implementation(ItemComponent, Result.bStackable? Result.TotalRoomToFill: 0);
	}
}

void UINV_InventoryComponent::Server_AddNewItem_Implementation(UINV_ItemComponent* ItemComponent, int32 StackCount)
{
	UINV_InventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem);
	}

	// TODO: Tell the item component to destroy its owning actor
}

void UINV_InventoryComponent::Server_AddStacksToItem_Implementation(UINV_ItemComponent* ItemComponent, int32 StackCount,
	int32 Remainder)
{
}


// Called when the game starts
void UINV_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}

void UINV_InventoryComponent::ToggleInventoryMenu()
{
	if (!bInventoryMenuOpen)
	{
		OpenInventoryMenu();
	}
	
	else
	{
		CloseInventoryMenu();
	}
}

void UINV_InventoryComponent::AddReplicatedSubObj(UObject* SubObject)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObject))
	{
		AddReplicatedSubObject(SubObject);	
	}	
}

void UINV_InventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("Inventory Component Should have a local APlayerController as Owner"));

	if (!OwningPlayerController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UINV_InventoryBase>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	CloseInventoryMenu();
}

void UINV_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;
	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);

}

void UINV_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}


