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

	virtual void Tick(float DeltaSeconds) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

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

protected: // Function
	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;

private: // Function
	double GetWorldTime() const;

private: // Property

	double StartLevelTime = 0.0;

	UPROPERTY(EditDefaultsOnly, Category = "Countdown")
	double TotalTime_Warmup = 10.0;

	UPROPERTY(EditDefaultsOnly, Category = "Countdown")
	double TotalTime_Match = 20.0;
};
