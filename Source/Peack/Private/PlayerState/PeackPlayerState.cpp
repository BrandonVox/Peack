// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

#include "Net/UnrealNetwork.h"


void APeackPlayerState::SetDeath(const float NewDeath)
{
	Death = NewDeath;
}

APeackPlayerState::APeackPlayerState()
{
	NetUpdateFrequency = 5.0f;
	MinNetUpdateFrequency = 5.0f;
}

// Server
void APeackPlayerState::AddOne_Score()
{
	SetScore(GetScore() + 1.0f);

	if (IsLocallyControlled())
	{
		UpdateText_Score();
	}
}

void APeackPlayerState::AddOne_Death()
{
	SetDeath(GetDeath() + 1.0f);

	if (IsLocallyControlled())
	{
		UpdateText_Death();
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

// Client: owning this player state
void APeackPlayerState::OnRep_Death()
{
	if (IsLocallyControlled())
	{
		UpdateText_Death();
	}
}

void APeackPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APeackPlayerState, bReady, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APeackPlayerState, Death, COND_OwnerOnly);
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

void APeackPlayerState::UpdateText_Death()
{
	if (PeackPlayerController == nullptr)
	{
		PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	}

	if (PeackPlayerController)
	{
		PeackPlayerController->UpdateText_Death(Death);
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
