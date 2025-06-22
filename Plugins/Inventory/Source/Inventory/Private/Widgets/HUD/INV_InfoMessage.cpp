// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/INV_InfoMessage.h"

#include "Components/TextBlock.h"

void UINV_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageShow();
}

void UINV_InfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle,
		[this]()
		{
			MessageHide();
			bIsMessageActive = false;
		},
		MessageLifeTime,
		false
		);
}
