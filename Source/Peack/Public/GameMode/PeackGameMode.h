// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PeackGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackGameMode : public AGameMode
{
	GENERATED_BODY()

public: // Function

	APeackGameMode();

	void HandleCharacterDead
	(
		AController* InstigatorController,
		AController* VictimController
	);

	void RequestRespawn
	(
		ACharacter* GivenCharacter,
		AController* GivenController
	);
};
