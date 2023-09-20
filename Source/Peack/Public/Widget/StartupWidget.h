// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartupWidget.generated.h"

class UButton;

UCLASS()
class PEACK_API UStartupWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_FindSessions;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_CreateSession;
};
