// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PeackPlayerController.h"
#include "Widget/CharacterWidget.h"

#include "Widget/PlayerStateWidget.h"

#include "PlayerState/PeackPlayerState.h"

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
	// total time - world time
	if (IsLocalController())
	{
		double TimeLeft = TotalTime_Match - GetWorldTime_Server();

		int CurrentCountdown = FMath::CeilToInt(TimeLeft);

		if (CurrentCountdown != LastCountdown)
		{
			UpdateText_Countdown(CurrentCountdown);
			LastCountdown = CurrentCountdown;
		}
	}

	// Server, 1 2 3
	// Client: 1
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



void APeackPlayerController::UpdateText_Countdown(int TimeLeft)
{
	if (Widget_PlayerState)
	{
		Widget_PlayerState->UpdateText_Countdown(TimeLeft);
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

// Local: Owning player state
void APeackPlayerController::PlayerStateReady(APeackPlayerState* GivenPlayerState)
{
	if (GivenPlayerState)
	{
		CreateWidget_PlayerState();
		UpdateText_Score(GivenPlayerState->GetScore());

		UpdateText_Death(GivenPlayerState->GetDeath());
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