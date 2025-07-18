// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/INV_CompositeBase.h"

void UINV_CompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UINV_CompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
