// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PeackPlayerController.h"
#include "Widget/CharacterWidget.h"

#include "Widget/PlayerStateWidget.h"

#include "PlayerState/PeackPlayerState.h"




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

void APeackPlayerController::UpdateText_Score(float GivenScore)
{
	if (Widget_PlayerState)
	{
		Widget_PlayerState->UpdateText_Score(GivenScore);
	}
}

// Local: Owning player state
void APeackPlayerController::PlayerStateReady(APeackPlayerState* GivenPlayerState)
{
	if (GivenPlayerState)
	{
		CreateWidget_PlayerState();
		UpdateText_Score(GivenPlayerState->GetScore());
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