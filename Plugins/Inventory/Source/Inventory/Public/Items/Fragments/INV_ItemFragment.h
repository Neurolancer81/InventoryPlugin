#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


#include "INV_ItemFragment.generated.h"

class UINV_InventoryComponent;

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
	

private:
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta=(Categories="FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
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
struct FINV_ImageFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const {return Icon;}
	void SetIcon(UTexture2D* InIcon) {Icon = InIcon;}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconSize{44.f, 44.f};
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

USTRUCT(BlueprintType)
struct FINV_ConsumableFragment : public FINV_ItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) {}
	
};

USTRUCT(BlueprintType)
struct FINV_HealthPotionFragment : public FINV_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float HealAmount = 20.f;

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) override;
};

USTRUCT(BlueprintType)
struct FINV_ManaPotionFragment : public FINV_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float ManaAmount = 20.f;

	virtual void OnConsume(UINV_InventoryComponent* InventoryComponent) override;
};