// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

#include "Net/UnrealNetwork.h"


APeackPlayerState::APeackPlayerState()
{
	NetUpdateFrequency = 5.0f;
	MinNetUpdateFrequency = 5.0f;
}

// Server
void APeackPlayerState::AddOne_Score()
{
	SetScore(GetScore() + 1.0f);
	// Server (Score) -> Client
	// OnRep Override

	// Server
	// PC owning this player controller?
	if (IsLocallyControlled())
	{
		UpdateText_Score();
	}
}



// Client
void APeackPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	if (IsLocallyControlled())
	{
		UpdateText_Score();
	}
}

void APeackPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APeackPlayerState, bReady, COND_OwnerOnly);

	// Server -> Client
	// Client owning this player state
}

void APeackPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		bReady = true;
		if (IsLocallyControlled())
		{
			OnRep_Ready();
		}
	}
}

void APeackPlayerState::UpdateText_Score()
{
	if (PeackPlayerController == nullptr)
	{
		PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	}

	if (PeackPlayerController)
	{
		PeackPlayerController->UpdateText_Score(GetScore());
	}
}

// Client owning this player state
void APeackPlayerState::OnRep_Ready()
{
	PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	if (PeackPlayerController)
	{
		PeackPlayerController->PlayerStateReady(this);
	}
}

bool APeackPlayerState::IsLocallyControlled() const
{
	AController* MyController = GetOwningController();
	return (MyController && MyController->IsLocalController());
}
