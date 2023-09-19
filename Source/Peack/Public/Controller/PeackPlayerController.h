// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;

class UPlayerStateWidget;

class APeackPlayerState;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public: // Function

	virtual void ReceivedPlayer() override;


	virtual void Tick(float DeltaSeconds) override;

	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);

	void PlayerStateReady(APeackPlayerState* GivenPlayerState);
	void UpdateBar_Health(float Health, float MaxHealth);
	void CreateWidget_Character();

	void CreateWidget_PlayerState();

private: // Function

	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(double RequestTimeFromClient);

	UFUNCTION(Client, Reliable)
	void Client_ReportServerTimeToClient
	(
		double RequestTimeFromClient,
		double ReceiveTimeFromServer
	);

	void UpdateText_Countdown(int TimeLeft);

	double GetWorldTime() const;
	double GetWorldTime_Server() const;

private: // Property
	/*
	* Countdown
	*/

	double TotalTime_Match = 20.0;

	int LastCountdown = 0;

	double Delta_Client_Server = 0.0;


	/*
	* Character Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UCharacterWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;

	/*
	* PlayerState Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPlayerStateWidget> WidgetClass_PlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStateWidget> Widget_PlayerState;
};
