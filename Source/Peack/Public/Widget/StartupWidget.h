// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartupWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PEACK_API UStartupWidget : public UUserWidget
{
	GENERATED_BODY()

public: // Function
	virtual bool Initialize() override;

private: // Function

	void ShowNotify
	(
		const FString& NotifyString,
		const FLinearColor& NotifyColor
	);



	void InputMode_UI();

	UFUNCTION()
	void OnClickButton_FindSessions();

	UFUNCTION()
	void OnClickButton_CreateSession();



private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_FindSessions;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_CreateSession;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Notify;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowNotifyAnimation;
};
