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

protected:
	virtual void BeginPlay() override;

private: // Function
	bool IsLocallyControlled() const;

private: // Property
	bool bReady = false;
};
