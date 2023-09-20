// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MultiplayerSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void UMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}

	if (SessionInterface.IsValid())
	{

		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject
		(
			this,
			&UMultiplayerSubsystem::OnCreateSessionComplete
		);

		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject
		(
			this,
			&UMultiplayerSubsystem::OnDestroySessionComplete
		);

	}
}

void UMultiplayerSubsystem::CreateSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = 20;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bIsDedicated = false;
	SessionSettings.BuildUniqueId = 1;


	// Destroy Existed Session
	if(SessionInterface->GetNamedSession(NAME_GameSession))
	{
		SessionInterface->DestroySession(NAME_GameSession);
		return;
	}


	SessionInterface->CreateSession
	(
		0,
		NAME_GameSession,
		SessionSettings
	);
}

void UMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (CreateSessionDoneDelegate.IsBound())
	{
		CreateSessionDoneDelegate.Broadcast(bWasSuccessful);
	}

	if (bWasSuccessful)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(Path_FirstMap);
		}
	}
}

void UMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (DestroySessionDoneDelegate.IsBound())
	{
		DestroySessionDoneDelegate.Broadcast(bWasSuccessful);
	}
}