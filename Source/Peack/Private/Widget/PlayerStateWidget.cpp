// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerStateWidget.h"

#include "Components/TextBlock.h"

void UPlayerStateWidget::UpdateText_Score(float GivenScore)
{
	FString NewString = FString::Printf(
		TEXT("%d"),
		FMath::FloorToInt(GivenScore)
	);

	if (Text_Score)
	{
		Text_Score->SetText(FText::FromString(NewString));
	}
}
