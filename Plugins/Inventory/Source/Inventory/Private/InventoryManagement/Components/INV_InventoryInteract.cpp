// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/INV_InventoryInteract.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/INV_HighlightableStatciMesh.h"
#include "InventoryManagement/Components/INV_InventoryComponent.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/INV_HUDWidget.h"


UINV_InventoryInteract::UINV_InventoryInteract()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	TraceLength = 500.0f;
	TraceChannel = ECC_GameTraceChannel1;

}


// Called when the game starts
void UINV_InventoryInteract::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayerController->GetLocalPlayer());

	if(IsValid(Subsystem))
	{
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	SetupInputComponent();

	InventoryComponent = OwningPlayerController->FindComponentByClass<UINV_InventoryComponent>();
	CreateHUDWidget();
}

void UINV_InventoryInteract::TickComponent(float DeltaTime, ELevelTick TickType,
										   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForItem();

}

void UINV_InventoryInteract::SetupInputComponent()
{
	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(OwningPlayerController->InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction,
		ETriggerEvent::Started,
		this,
		&UINV_InventoryInteract::PrimaryInteract
		);

	EnhancedInputComponent->BindAction(ToggleInventoryAction,
		ETriggerEvent::Started,
		this,
		&UINV_InventoryInteract::ToggleInventory
		);
	
}

void UINV_InventoryInteract::PrimaryInteract()
{
	UE_LOG(LogTemp, Display, TEXT("Primary Interact"));
}

void UINV_InventoryInteract::CreateHUDWidget()
{
	if (!OwningPlayerController->IsLocalController()) return;

	HUDWidget = CreateWidget<UINV_HUDWidget>(OwningPlayerController.Get(), HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
	
}

void UINV_InventoryInteract::ToggleInventory()
{
	if (!InventoryComponent.IsValid())
	{	
		UE_LOG(LogTemp, Warning, TEXT("ToggleInventory not working, Inventory Component Invalid"));
		return;
	}
	InventoryComponent->ToggleInventoryMenu();
}

void UINV_InventoryInteract::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

	FVector2D ViewportSize = FVector2D::ZeroVector;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector Forward;

	if(!UGameplayStatics::DeprojectScreenToWorld(OwningPlayerController.Get(), ViewportCenter,
		TraceStart, Forward)) return;
	const FVector TraceEnd = TraceStart + Forward * TraceLength;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UINV_HighlightableStatciMesh::StaticClass());
		if (IsValid(Highlightable))
		{
			IINV_Highlightable::Execute_Highlight(Highlightable);
		}

		UINV_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UINV_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
	}

	if (LastActor.IsValid())
	{
		UActorComponent* HighLightable = LastActor->FindComponentByInterface(UINV_HighlightableStatciMesh::StaticClass());
		if (IsValid(HighLightable))
		{
			IINV_Highlightable::Execute_UnHighlight(HighLightable);
		}
	}
}