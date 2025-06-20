﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "INV_WidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UINV_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
};
