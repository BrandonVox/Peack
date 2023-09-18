// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PeackGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

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
