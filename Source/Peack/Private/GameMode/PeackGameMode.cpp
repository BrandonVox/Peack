// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PeackGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "PlayerState/PeackPlayerState.h"

// Server
void APeackGameMode::HandleCharacterDead(AController* InstigatorController, AController* VictimController)
{
	// InstigatorController
	// peack player state
	// add 1 score (instigator)
	APeackPlayerState* PlayerState_Instigator =
		InstigatorController->GetPlayerState<APeackPlayerState>();

	if (PlayerState_Instigator)
	{
		PlayerState_Instigator->AddOne_Score();
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
