// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public: // Function
	void UpdateBar_Health(float Health, float MaxHealth);
	void CreateWidget_Character();

private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCharacterWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;

	// Character Widget
};
