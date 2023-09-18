// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;

class UPlayerStateWidget;

class APeackPlayerState;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public: // Function
	void UpdateText_Score(float GivenScore);

	void PlayerStateReady(APeackPlayerState* GivenPlayerState);
	void UpdateBar_Health(float Health, float MaxHealth);
	void CreateWidget_Character();

	void CreateWidget_PlayerState();

private: // Function

private: // Property
	/*
	* Character Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCharacterWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;

	/*
	* PlayerState Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPlayerStateWidget> WidgetClass_PlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStateWidget> Widget_PlayerState;
};
