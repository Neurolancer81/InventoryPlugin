// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/INV_HighlightableStatciMesh.h"

void UINV_HighlightableStatciMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UINV_HighlightableStatciMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
