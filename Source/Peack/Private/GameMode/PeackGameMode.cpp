// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PeackGameMode.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "PlayerState/PeackPlayerState.h"

#include "Controller/PeackPlayerController.h"

#include "GameState/PeackGameState.h"


namespace MatchState
{
	const FName ShowResult = FName(TEXT("ShowResult"));
}

void APeackGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartLevelTime = GetWorldTime();
}

// Server
void APeackGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Lobby Map: 50s
	// First Map (Peack Game Mode)
	// Begin Play (Game Mode)

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		//                     10                50   = -40
		double TimeLeft = TotalTime_Warmup - GetWorldTime();
		TimeLeft += StartLevelTime;

		if (TimeLeft <= 0.0)
		{
			StartMatch();
		}
	}
	else if (GetMatchState() == MatchState::InProgress)
	{
		double TimeLeft = TotalTime_Match - GetWorldTime();
		TimeLeft += StartLevelTime;
		TimeLeft += TotalTime_Warmup;

		if (TimeLeft <= 0.0)
		{
			SetMatchState(MatchState::ShowResult);
		}
	}
}

void APeackGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (APeackPlayerController* PPC = Cast<APeackPlayerController>(NewPlayer))
	{
		PPC->GameModeSendInformations(
			StartLevelTime,
			GetMatchState(),
			TotalTime_Warmup,
			TotalTime_Match,
			TotalTime_ShowResult
		);
	}
}

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

		if (APeackGameState* PeackGameState = GetGameState<APeackGameState>())
		{
			PeackGameState->UpdateMVP(PlayerState_Instigator);
		}
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
			PPC->GameModeChangeMatchState(GetMatchState());
		}
	}

	// event game mode reveice player controller (client)
}

double APeackGameMode::GetWorldTime() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0.0;
	}

	return World->GetTimeSeconds();
}
