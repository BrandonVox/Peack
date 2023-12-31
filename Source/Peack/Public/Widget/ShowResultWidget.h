// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CountdownWidget.h"
#include "ShowResultWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UShowResultWidget : public UCountdownWidget
{
	GENERATED_BODY()

public:
	void UpdateText_WinnerName(const FString& WinnerNameString);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_WinnerName;
};
