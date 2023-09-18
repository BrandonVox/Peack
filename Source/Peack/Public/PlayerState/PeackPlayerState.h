// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PeackPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackPlayerState : public APlayerState
{
	GENERATED_BODY()

public: // Function
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private: // Function
	UFUNCTION()
	void OnRep_Ready();

	bool IsLocallyControlled() const;

private: // Property

	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bReady = false;
};
