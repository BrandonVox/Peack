// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PeackPlayerController.h"
#include "Widget/CharacterWidget.h"

#include "Widget/PlayerStateWidget.h"

#include "PlayerState/PeackPlayerState.h"

void APeackPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// total time - world time
	if (IsLocalController())
	{
		double TimeLeft = TotalTime_Match - GetWorldTime();

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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			4.0f,
			FColor::Blue,
			TEXT("UpdateText_Countdown")
		);
	}



	if (Widget_PlayerState)
	{
		Widget_PlayerState->UpdateText_Countdown(TimeLeft);
	}
}

double APeackPlayerController::GetWorldTime() const
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0.0;
	}
	
	return World->GetTimeSeconds();
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