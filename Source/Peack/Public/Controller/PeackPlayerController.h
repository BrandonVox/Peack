// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()
public: // Function
	void CreateWidget_Character();
private: // Property
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget_Character;
};
