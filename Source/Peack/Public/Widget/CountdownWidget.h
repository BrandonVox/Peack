// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UCountdownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateText_Countdown(int TimeLeft);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Countdown;
};
