// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/INV_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Widgets/HUD/INV_HUDWidget.h"


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
