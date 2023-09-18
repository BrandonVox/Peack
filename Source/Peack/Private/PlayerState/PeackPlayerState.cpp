// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

void APeackPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		bReady = true;
		if (IsLocallyControlled())
		{
			APeackPlayerController* PeackPlayerController
				= Cast<APeackPlayerController>(GetOwningController());
			if (PeackPlayerController)
			{
				PeackPlayerController->PlayerStateReady();
			}

		}
	}
}

bool APeackPlayerState::IsLocallyControlled() const
{
	AController* MyController = GetOwningController();
	return (MyController && MyController->IsLocalController());
}
