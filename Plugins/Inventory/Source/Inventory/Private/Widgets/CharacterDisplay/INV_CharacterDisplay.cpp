// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterDisplay/INV_CharacterDisplay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "EquipmentManagement/ProxyMesh/INV_ProxyMesh.h"
#include "Kismet/GameplayStatics.h"

FReply UINV_CharacterDisplay::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UINV_CharacterDisplay::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CurrentMousePosition  = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	LastMousePosition    = CurrentMousePosition;
	bIsDragging = true;
	return FReply::Handled();
}

void UINV_CharacterDisplay::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bIsDragging = false;
}

void UINV_CharacterDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AINV_ProxyMesh::StaticClass(), Actors);

	if (!Actors.IsValidIndex(0)) return;
	AINV_ProxyMesh* Mesh = Cast<AINV_ProxyMesh>(Actors[0]);
	if (!Mesh) return;

	ProxyMesh = Mesh->GetMesh();

	
	
}

void UINV_CharacterDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsDragging) return;
	if (!ProxyMesh.IsValid()) return;

	LastMousePosition = CurrentMousePosition;
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	const float HorizontalOffset = LastMousePosition.X - CurrentMousePosition.X;
	ProxyMesh->AddRelativeRotation(FRotator(0.0, HorizontalOffset, 0.0f));
	
	
}
