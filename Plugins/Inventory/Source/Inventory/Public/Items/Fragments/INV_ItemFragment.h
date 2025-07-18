#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Storage/Nodes/FileEntry.h"
#include "StructUtils/InstancedStruct.h"


#include "INV_ItemFragment.generated.h"

class UINV_InventoryComponent;
class UINV_CompositeBase;

USTRUCT(BlueprintType)
struct FINV_ItemFragment
{
	GENERATED_BODY()

	FINV_ItemFragment() {};
	/** Rule of fives **/
	// Copy constructor
	FINV_ItemFragment(const FINV_ItemFragment&) = default;

	// Assigment Operator
	FINV_ItemFragment& operator=(const FINV_ItemFragment&) = default;

	// Move Constructor
	FINV_ItemFragment(FINV_ItemFragment&&) = default;
	
	// Move Assigment Operator
	FINV_ItemFragment& operator=(FINV_ItemFragment&&) = default;
	
	// Default destructor
	virtual ~FINV_ItemFragment() {}


	FGameplayTag GetFragmentTag() const {return FragmentTag;}
	void SetFragmentTag(FGameplayTag Tag) {FragmentTag = Tag;}
	virtual void Manifest() {}
	

private:
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(Categories="FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FINV_InventoryItemFragment : public FINV_ItemFragment
{
	GENERATED_BODY()
	
	virtual void Assimilate(UINV_CompositeBase* Composite) const;
	
protected:
	bool MatchesWidgetTag(const UINV_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FINV_GridFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const {return GridSize;}
	void SetGridSize(FIntPoint Size) {GridSize = Size;}

	float GetGridPadding() const {return GridPadding;}
	void SetGridPadding(float Padding) {GridPadding = Padding;}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1,1	};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{0.f};
};

USTRUCT(BlueprintType)
struct FINV_ImageFragment : public FINV_InventoryItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const {return Icon;}
	virtual void Assimilate(UINV_CompositeBase* Composite) const override;
	void SetIcon(UTexture2D* InIcon) {Icon = InIcon;}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconSize{44.f, 44.f};
};

USTRUCT(BlueprintType)
struct FINV_TextFragment : public FINV_InventoryItemFragment
{
	GENERATED_BODY()

	FText GetText() const {return FragmentText;}
	void SetText(const FText& Text) {FragmentText = Text;}

	virtual void Assimilate(UINV_CompositeBase* Composite) const override;
	

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;	
};

USTRUCT(BlueprintType)
struct FINV_LabeledNumberFragment : public FINV_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Manifest() override;
	virtual void Assimilate(UINV_CompositeBase* Composite) const override;
	float GetValue() const {return Value;}
	
	// When Manifesting for the first time, the fragment will randomize but when equipped and dropped we should not re-randomize
	bool bRandomizeOnManifest{true};
	

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MinValue{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MaxValue{1.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxDigits{1};
};

USTRUCT(BlueprintType)
struct FINV_StackableFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const {return MaxStackSize;}
	void SetMaxStackSize(int32 Size) {MaxStackSize = Size;}

	int32 GetStackCount() const {return StackCount;}
	void SetStackCount(int32 Count) {StackCount = Count;}
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int StackCount{1};
};

// Consumable fragments

USTRUCT(BlueprintType)
struct FINV_ConsumeModifier : public FINV_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) {}
};

USTRUCT(BlueprintType)
struct FINV_ConsumableFragment : public FINV_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UINV_CompositeBase* Composite) const override;
	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent);
	virtual void Manifest() override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FINV_ConsumeModifier>> ConsumeModifiers;
	
};

USTRUCT(BlueprintType)
struct FINV_HealthPotionFragment : public FINV_ConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) override;
};

USTRUCT(BlueprintType)
struct FINV_ManaPotionFragment : public FINV_ConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) override;
};

// Equipment based fragments

//
USTRUCT(BlueprintType)
struct FINV_EquipModifier : public FINV_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnEquip(UINV_InventoryComponent* InventoryComponent) {}
	virtual void OnUnEquip(UINV_InventoryComponent* InventoryComponent) {}

	
};

USTRUCT(BlueprintType)
struct FINV_StrengthModifier : public FINV_EquipModifier
{
	GENERATED_BODY()

	virtual void OnEquip(UINV_InventoryComponent* InventoryComponent) override;
	virtual void OnUnEquip(UINV_InventoryComponent* InventoryComponent) override;

	
};


USTRUCT(BlueprintType)
struct FINV_EquipmentFragment : public FINV_InventoryItemFragment
{
	GENERATED_BODY()
	bool bEquipped{false};
	void OnEquip(UINV_InventoryComponent* InventoryComponent);
	void OnUnEquip(UINV_InventoryComponent* InventoryComponent);
	virtual void Assimilate(UINV_CompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FINV_EquipModifier>> EquipModifiers;

	

	
};