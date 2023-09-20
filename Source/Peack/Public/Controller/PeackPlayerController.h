// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;

class UPlayerStateWidget;

class APeackPlayerState;

class UWarmupWidget;

class UShowResultWidget;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public: // Function

	void ToggleText_MVP(bool bVisible);

	void GameModeSendInformations(
		const double SLT,
		const FName GivenMatchState,
		const double TotalWarmupTime,
		const double TotalMatchTime,
		const double TotalShowResultTime
	);

	UFUNCTION(Client, Reliable)
	void Client_GameModeSendInformations
	(
		const double SLT,
		const FName GivenMatchState,
		const double TotalWarmupTime,
		const double TotalMatchTime,
		const double TotalShowResultTime
	);

	void GameModeChangeMatchState(const FName NewMatchState);

	UFUNCTION(Client, Reliable)
	void Client_GameModeChangeMatchState(const FName NewMatchState);


	virtual void ReceivedPlayer() override;


	virtual void Tick(float DeltaSeconds) override;

	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);

	void PlayerStateReady(APeackPlayerState* GivenPlayerState);
	void UpdateBar_Health(float Health, float MaxHealth);
	void CreateWidget_Character();

	void CreateWidget_PlayerState();

	void CreateWidget_Warmup();

	void CreateWidget_ShowResult();

private: // Function

	void UpdateText_WinnerName(const FString& WinnerNameString);

	void ShowWinnerName();

	void UpdateCountdown();

	void UpdateCountdown_Warmup();
	void UpdateCountdown_InMatch();
	void UpdateCountdown_ShowResult();


	void HandleMatchState(const FName GivenMatchState);


	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(double RequestTimeFromClient);

	UFUNCTION(Client, Reliable)
	void Client_ReportServerTimeToClient
	(
		double RequestTimeFromClient,
		double ReceiveTimeFromServer
	);

	double GetWorldTime() const;
	double GetWorldTime_Server() const;

private: // Property

	FName CurrentMatchState;

	UPROPERTY()
	TObjectPtr<APeackPlayerState> PeackPlayerState;

	/*
	* Countdown
	*/
	double StartLevelTime = 0.0;
	double TotalTime_Warmup = 0.0;
	double TotalTime_Match = 0.0;
	double TotalTime_ShowResult = 0.0;

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

	/*
	* Warmup Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UWarmupWidget> WidgetClass_Warmup;

	UPROPERTY()
	TObjectPtr<UWarmupWidget> Widget_Warmup;

	/*
	* ShowResult Widget
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UShowResultWidget> WidgetClass_ShowResult;

	UPROPERTY()
	TObjectPtr<UShowResultWidget> Widget_ShowResult;
};
