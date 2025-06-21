// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "INV_InventoryInteract.generated.h"


class UINV_HUDWidget;
class UInputAction;
class UInputMappingContext;
class UINV_InventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UINV_InventoryInteract : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UINV_InventoryInteract();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void SetupInputComponent();
	

private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();

	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<UINV_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UINV_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UINV_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
};
