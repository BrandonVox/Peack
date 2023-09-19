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

void UPlayerStateWidget::UpdateText_Death(float GivenDeath)
{
	FString NewString = FString::Printf(
		TEXT("%d"),
		FMath::FloorToInt(GivenDeath)
	);

	if (Text_Death)
	{
		Text_Death->SetText(FText::FromString(NewString));
	}
}

void UPlayerStateWidget::ToggleText_MVP(bool bVisible)
{
	if (Text_MVP == nullptr)
	{
		return;
	}

	if (bVisible)
	{
		Text_MVP->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_MVP->SetVisibility(ESlateVisibility::Hidden);
	}
}


