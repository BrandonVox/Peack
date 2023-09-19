// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PeackPlayerController.h"
#include "Widget/CharacterWidget.h"

#include "Widget/PlayerStateWidget.h"

#include "Widget/WarmupWidget.h"

#include "Widget/ShowResultWidget.h"


#include "PlayerState/PeackPlayerState.h"

#include "GameMode/PeackGameMode.h"

// Server
void APeackPlayerController::GameModeSendInformations(
	const double SLT,
	const FName GivenMatchState,
	const double TotalWarmupTime,
	const double TotalMatchTime,
	const double TotalShowResultTime
)
{
	// Client RPC
	Client_GameModeSendInformations
	(
		SLT,
		GivenMatchState,
		TotalWarmupTime,
		TotalMatchTime,
		TotalShowResultTime
	);
}

// Owning this player controller (Server, Client)
void APeackPlayerController::Client_GameModeSendInformations_Implementation
(
	const double SLT,
	const FName GivenMatchState,
	const double TotalWarmupTime,
	const double TotalMatchTime,
	const double TotalShowResultTime
) // Implementation
{
	StartLevelTime = SLT;
	TotalTime_Warmup = TotalWarmupTime;
	TotalTime_Match = TotalMatchTime;
	TotalTime_ShowResult = TotalShowResultTime;


	// Widget....
	HandleMatchState(GivenMatchState);
}

// Server
void APeackPlayerController::GameModeChangeMatchState(const FName NewMatchState)
{
	// widget
	// code run on pc owning this player controller
	// client rpc
	Client_GameModeChangeMatchState(NewMatchState);
}

// Local: Pc Owning this player controller
void APeackPlayerController::Client_GameModeChangeMatchState_Implementation(const FName NewMatchState) // Implementation
{
	HandleMatchState(NewMatchState);
}



void APeackPlayerController::HandleMatchState(const FName GivenMatchState)
{
	if (CurrentMatchState == GivenMatchState)
	{
		return;
	}

	CurrentMatchState = GivenMatchState;

	if (GivenMatchState == MatchState::WaitingToStart)
	{
		CreateWidget_Warmup();
	}
	else if (GivenMatchState == MatchState::InProgress)
	{
		if (Widget_Warmup)
		{
			Widget_Warmup->RemoveFromParent();
		}
		if (PeackPlayerState)
		{
			CreateWidget_PlayerState();
			UpdateText_Score(PeackPlayerState->GetScore());
			UpdateText_Death(PeackPlayerState->GetDeath());
		}
	}
	else if (GivenMatchState == MatchState::ShowResult)
	{
		if (Widget_PlayerState)
		{
			Widget_PlayerState->RemoveFromParent();
		}
		CreateWidget_ShowResult();
	}
}

// Local: Owning player state
void APeackPlayerController::PlayerStateReady(APeackPlayerState* GivenPlayerState)
{
	PeackPlayerState = GivenPlayerState;

	if (CurrentMatchState == MatchState::InProgress)
	{
		if (PeackPlayerState)
		{
			CreateWidget_PlayerState();
			UpdateText_Score(PeackPlayerState->GetScore());
			UpdateText_Death(PeackPlayerState->GetDeath());
		}
	}
}


// Client, Server
// Server: 3
// Client: 1
// Client: 1
void APeackPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (!HasAuthority() && IsLocalController())
	{
		Server_RequestServerTime(GetWorldTime());
	}
}

// Server
void APeackPlayerController::Server_RequestServerTime_Implementation(double RequestTimeFromClient) // Implementation
{
	// Server -> Client
	// Receive Time
	// Client RPC
	Client_ReportServerTimeToClient(
		RequestTimeFromClient,
		GetWorldTime()
	);
}

// Client
void APeackPlayerController::Client_ReportServerTimeToClient_Implementation(double RequestTimeFromClient, double ReceiveTimeFromServer) // Implementation
{
	double RoundTripTime = GetWorldTime() - RequestTimeFromClient;

	double CurrentServerTime = ReceiveTimeFromServer + (0.5 * RoundTripTime);

	Delta_Client_Server = CurrentServerTime - GetWorldTime();
}

void APeackPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController())
	{
		UpdateCountdown();
	}
}

void APeackPlayerController::UpdateCountdown()
{
	if (CurrentMatchState == MatchState::WaitingToStart)
	{
		UpdateCountdown_Warmup();
	}
	else if (CurrentMatchState == MatchState::InProgress)
	{
		UpdateCountdown_InMatch();
	}
	else if (CurrentMatchState == MatchState::ShowResult)
	{
		UpdateCountdown_ShowResult();
	}
}

void APeackPlayerController::UpdateCountdown_Warmup()
{
	double TimeLeft = TotalTime_Warmup - GetWorldTime_Server();
	TimeLeft += StartLevelTime;

	int CurrentCountdown = FMath::CeilToInt(TimeLeft);

	if (CurrentCountdown != LastCountdown)
	{
		if (Widget_Warmup)
		{
			Widget_Warmup->UpdateText_Countdown(CurrentCountdown);
		}

		LastCountdown = CurrentCountdown;
	}
}

void APeackPlayerController::UpdateCountdown_InMatch()
{
	// 15s
	// 5s
	// 10s
	double TimeLeft = TotalTime_Match - GetWorldTime_Server();
	TimeLeft += StartLevelTime;
	TimeLeft += TotalTime_Warmup;

	int CurrentCountdown = FMath::CeilToInt(TimeLeft);

	if (CurrentCountdown != LastCountdown)
	{
		if (Widget_PlayerState)
		{
			Widget_PlayerState->UpdateText_Countdown(CurrentCountdown);
		}

		LastCountdown = CurrentCountdown;
	}
}

void APeackPlayerController::UpdateCountdown_ShowResult()
{
	double TimeLeft = TotalTime_ShowResult - GetWorldTime_Server();
	TimeLeft += StartLevelTime;
	TimeLeft += TotalTime_Warmup;
	TimeLeft += TotalTime_Match;

	int CurrentCountdown = FMath::CeilToInt(TimeLeft);

	if (CurrentCountdown != LastCountdown)
	{
		if (Widget_ShowResult)
		{
			Widget_ShowResult->UpdateText_Countdown(CurrentCountdown);
		}

		LastCountdown = CurrentCountdown;
	}
}

void APeackPlayerController::CreateWidget_Character()
{
	if (Widget_Character)
	{
		return;
	}

	Widget_Character = CreateWidget<UCharacterWidget>(this, WidgetClass_Character);
	if (Widget_Character)
	{
		Widget_Character->AddToViewport();
	}
}

void APeackPlayerController::CreateWidget_PlayerState()
{
	if (Widget_PlayerState)
	{
		return;
	}

	Widget_PlayerState = CreateWidget<UPlayerStateWidget>(this, WidgetClass_PlayerState);
	if (Widget_PlayerState)
	{
		Widget_PlayerState->AddToViewport();
	}
}

void APeackPlayerController::CreateWidget_Warmup()
{
	if (Widget_Warmup)
	{
		return;
	}

	Widget_Warmup = CreateWidget<UWarmupWidget>(this, WidgetClass_Warmup);
	if (Widget_Warmup)
	{
		Widget_Warmup->AddToViewport();
	}
}

void APeackPlayerController::CreateWidget_ShowResult()
{
	if (Widget_ShowResult)
	{
		return;
	}

	Widget_ShowResult = CreateWidget<UShowResultWidget>(this, WidgetClass_ShowResult);
	if (Widget_ShowResult)
	{
		Widget_ShowResult->AddToViewport();
	}
}

// Local
double APeackPlayerController::GetWorldTime() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0.0;
	}
	
	return World->GetTimeSeconds();
}

double APeackPlayerController::GetWorldTime_Server() const
{
	return GetWorldTime() + Delta_Client_Server;
}



void APeackPlayerController::UpdateText_Score(float GivenScore)
{
	if (Widget_PlayerState)
	{
		Widget_PlayerState->UpdateText_Score(GivenScore);
	}
}

void APeackPlayerController::UpdateText_Death(float GivenDeath)
{
	if (Widget_PlayerState)
	{
		Widget_PlayerState->UpdateText_Death(GivenDeath);
	}
}



// update health bar
void APeackPlayerController::UpdateBar_Health(float Health, float MaxHealth)
{
	if (Widget_Character)
	{
		Widget_Character->UpdateBar_Health(Health, MaxHealth);
	}
}