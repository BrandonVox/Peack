// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/PeackGameState.h"

#include "PlayerState/PeackPlayerState.h"

APeackGameState::APeackGameState()
{
	NetUpdateFrequency = 10.0f;
	MinNetUpdateFrequency = 7.0f;
}

// Server
void APeackGameState::UpdateMVP(APeackPlayerState* InstigatorPlayerState)
{
	if (InstigatorPlayerState == nullptr)
	{
		return;
	}

	if (CurrentMVP == InstigatorPlayerState)
	{
		return;
	}

	if (CurrentMVP == nullptr)
	{
		CurrentMVP = InstigatorPlayerState;
		return;
	}

	if (InstigatorPlayerState->GetScore() > CurrentMVP->GetScore())
	{
		CurrentMVP = InstigatorPlayerState;
	}
}
