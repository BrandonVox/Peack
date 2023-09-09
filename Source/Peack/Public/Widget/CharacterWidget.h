// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterWidget.generated.h"

class UProgressBar;

UCLASS()
class PEACK_API UCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateBar_Health(float Health, float MaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_Health;
};
