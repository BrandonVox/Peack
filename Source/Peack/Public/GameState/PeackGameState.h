// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PeackGameState.generated.h"

class APeackPlayerState;

UCLASS()
class PEACK_API APeackGameState : public AGameState
{
	GENERATED_BODY()

public: // Function
	APeackGameState();

	void UpdateMVP(APeackPlayerState* InstigatorPlayerState);

public: // Property
	UPROPERTY()
	TObjectPtr<APeackPlayerState> CurrentMVP;
};
