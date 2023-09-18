// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

#include "Net/UnrealNetwork.h"

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

// Client owning this player state
void APeackPlayerState::OnRep_Ready()
{
	APeackPlayerController* PeackPlayerController
		= Cast<APeackPlayerController>(GetOwningController());
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
