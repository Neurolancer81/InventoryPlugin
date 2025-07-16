#include "Items/Fragments/INV_ItemFragment.h"

void FINV_HealthPotionFragment::OnConsume(UINV_InventoryComponent* InventoryComponent)
{
	// Get the stats component from the IC->PC->GetPawn()
	// This is where we will apply a gameplay effect

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Health Potion Consumed! Heal by: %f"),HealAmount));
}

void FINV_ManaPotionFragment::OnConsume(UINV_InventoryComponent* InventoryComponent)
{
	// Get the stats component from the IC->PC->GetPawn()
	// This is where we will apply a gameplay effect

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
		FString::Printf(TEXT("Health Potion Consumed! Heal by: %f"),ManaAmount));
}
