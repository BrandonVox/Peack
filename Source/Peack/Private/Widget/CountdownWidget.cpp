// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CountdownWidget.h"

#include "Components/TextBlock.h"


void UCountdownWidget::UpdateText_Countdown(int TimeLeft)
{
	FString NewString = FString::Printf(
		TEXT("%d"),
		TimeLeft
	);

	if (Text_Countdown)
	{
		Text_Countdown->SetText(FText::FromString(NewString));
	}
}