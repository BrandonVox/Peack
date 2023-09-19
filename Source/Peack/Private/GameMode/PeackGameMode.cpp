// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PeackGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

APeackGameMode::APeackGameMode()
{
	bDelayedStart = true;
}

// Server
void APeackGameMode::HandleCharacterDead(AController* InstigatorController, AController* VictimController)
{
	/*
	* Add 1 Score
	*/
	if (APeackPlayerState* PlayerState_Instigator = InstigatorController->GetPlayerState<APeackPlayerState>())
	{
		PlayerState_Instigator->AddOne_Score();
	}

	/*
	* Add 1 Death
	*/
	if (APeackPlayerState* PlayerState_Victim = VictimController->GetPlayerState<APeackPlayerState>())
	{
		PlayerState_Victim->AddOne_Death();
	}
}

// Server
void APeackGameMode::RequestRespawn
(
	ACharacter* GivenCharacter,
	AController* GivenController
)
{
	// tach controller ra khoi character
	if (GivenCharacter)
	{
		GivenCharacter->DetachFromControllerPendingDestroy();
	}

	TArray<AActor*> PlayerStartActors;
	UGameplayStatics::GetAllActorsOfClass
	(
		this,
		APlayerStart::StaticClass(),
		PlayerStartActors
	);

	// spawn new character at player start
	if (PlayerStartActors.IsValidIndex(0))
	{
		RestartPlayerAtPlayerStart(GivenController, PlayerStartActors[0]);
	}
}

// Server
void APeackGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	// thong bao cho tat ca player controller
	// match state changed

	for (FConstPlayerControllerIterator PCI = GetWorld()->GetPlayerControllerIterator(); PCI; ++PCI)
	{
		if (APeackPlayerController* PPC = Cast<APeackPlayerController>(*PCI))
		{
			PPC->MatchStateGameModeChanged(GetMatchState());
		}
	}
}
