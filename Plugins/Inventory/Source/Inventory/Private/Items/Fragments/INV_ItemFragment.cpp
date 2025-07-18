#include "Items/Fragments/INV_ItemFragment.h"

#include "Widgets/Composite/INV_CompositeBase.h"
#include "Widgets/Composite/INV_Leaf_Image.h"
#include "Widgets/Composite/INV_Leaf_LabeledValue.h"
#include "Widgets/Composite/INV_Leaf_Text.h"

bool FINV_InventoryItemFragment::MatchesWidgetTag(const UINV_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FINV_InventoryItemFragment::Assimilate(UINV_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

void FINV_ImageFragment::Assimilate(UINV_CompositeBase* Composite) const
{
	FINV_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UINV_Leaf_Image* Image = Cast<UINV_Leaf_Image>(Composite);
	if (!Image) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconSize);
	Image->SetImageSize(IconSize);
}

void FINV_TextFragment::Assimilate(UINV_CompositeBase* Composite) const
{
	FINV_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UINV_Leaf_Text* LeafText = Cast<UINV_Leaf_Text>(Composite);
	if (!LeafText) return;

	LeafText->SetText(FragmentText);
}

void FINV_LabeledNumberFragment::Manifest()
{
	FINV_InventoryItemFragment::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(MinValue, MaxValue);
	}
	bRandomizeOnManifest = false;
}

void FINV_LabeledNumberFragment::Assimilate(UINV_CompositeBase* Composite) const
{
	FINV_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UINV_Leaf_LabeledValue* LabeledValue = Cast<UINV_Leaf_LabeledValue>(Composite);
	if (!LabeledValue) return;
	
	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinDigits;
	Options.MaximumFractionalDigits = MaxDigits;
	
	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);
	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
	
}

void FINV_ConsumableFragment::Assimilate(UINV_CompositeBase* Composite) const
{
	FINV_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier: ConsumeModifiers)
	{
		Modifier.Get().Assimilate(Composite);
	}
}

void FINV_ConsumableFragment::OnConsume(UINV_InventoryComponent* InventoryComponent)
{
	for (auto& Modifier: ConsumeModifiers)
	{
		Modifier.GetMutable().OnConsume(InventoryComponent);
	}
}

void FINV_ConsumableFragment::Manifest()
{
	FINV_InventoryItemFragment::Manifest();
	for (auto& Modifier: ConsumeModifiers)
	{
		Modifier.GetMutable().Manifest();
	}
}

void FINV_HealthPotionFragment::OnConsume(UINV_InventoryComponent* InventoryComponent)
{
	// Get the stats component from the IC->PC->GetPawn()
	// This is where we will apply a gameplay effect

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Health Potion Consumed! Heal by: %f"), GetValue()));
}

void FINV_ManaPotionFragment::OnConsume(UINV_InventoryComponent* InventoryComponent)
{
	// Get the stats component from the IC->PC->GetPawn()
	// This is where we will apply a gameplay effect

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
		FString::Printf(TEXT("Health Potion Consumed! Heal by: %f"),GetValue()));
}
