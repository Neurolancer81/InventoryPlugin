// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/INV_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/INV_HighlightableStatciMesh.h"
#include "Items/Components/INV_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/INV_HUDWidget.h"


AINV_PlayerController::AINV_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0f;
	TraceChannel = ECC_GameTraceChannel1;
}

void AINV_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForItem();
}

void AINV_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if(IsValid(Subsystem))
	{
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	CreateHUDWidget();
}

void AINV_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction,
		ETriggerEvent::Started,
		this,
		&AINV_PlayerController::PrimaryInteract
		);
}

void AINV_PlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Display, TEXT("Primary Interact"));
}

void AINV_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UINV_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AINV_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize = FVector2D::ZeroVector;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector Forward;

	if(!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward)) return;
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
