// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CountdownWidget.h"
#include "PlayerStateWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UPlayerStateWidget : public UCountdownWidget
{
	GENERATED_BODY()

public:
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);

	void ToggleText_MVP(bool bVisible);


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Score;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Death;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_MVP;
};
