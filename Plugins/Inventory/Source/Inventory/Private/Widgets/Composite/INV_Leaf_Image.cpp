// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/INV_Leaf_Image.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UINV_Leaf_Image::SetImage(UTexture2D* InIcon) const
{
	Image_Icon->SetBrushFromTexture(InIcon);
}

void UINV_Leaf_Image::SetBoxSize(const FVector2D& InSize) const
{
	SizeBox_Icon->SetWidthOverride(InSize.X);
	SizeBox_Icon->SetHeightOverride(InSize.Y);
}

void UINV_Leaf_Image::SetImageSize(const FVector2D& InSize) const
{
	Image_Icon->SetDesiredSizeOverride(InSize);
}

FVector2D UINV_Leaf_Image::GetImageSize() const
{
	return Image_Icon->GetDesiredSize();
}
