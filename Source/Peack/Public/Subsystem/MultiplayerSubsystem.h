// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCreateSessionDoneDelegate, bool bWasSuccessful);

DECLARE_MULTICAST_DELEGATE_OneParam(FDestroySessionDoneDelegate, bool bWasSuccessful);

UCLASS()
class PEACK_API UMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: // Function

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSession();

private: // Function
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	
public: // Property
	FCreateSessionDoneDelegate CreateSessionDoneDelegate;
	FDestroySessionDoneDelegate DestroySessionDoneDelegate;

private: // Property
	IOnlineSessionPtr SessionInterface;

	// Listen Server
	FString Path_FirstMap = TEXT("/Game/Map/FirstMap?listen");
};
