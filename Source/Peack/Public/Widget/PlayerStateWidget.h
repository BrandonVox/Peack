// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateText_Score(float GivenScore);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Score;
};
