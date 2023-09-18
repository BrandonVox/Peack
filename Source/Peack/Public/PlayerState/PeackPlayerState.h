// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PeackPlayerState.generated.h"

class APeackPlayerController;

UCLASS()
class PEACK_API APeackPlayerState : public APlayerState
{
	GENERATED_BODY()

public: // Function

	void AddOne_Score();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private: // Function

	void UpdateText_Score();

	UFUNCTION()
	void OnRep_Ready();

	bool IsLocallyControlled() const;

private: // Property
	UPROPERTY()
	TObjectPtr<APeackPlayerController> PeackPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bReady = false;
};
