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

		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject
		(
			this,
			&UMultiplayerSubsystem::OnFindSessionsComplete
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

void UMultiplayerSubsystem::FindSessions()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (!SessionSearch.IsValid())
	{
		return;
	}

	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 10'000;

	SessionSearch->QuerySettings.Set
	(
		SEARCH_PRESENCE,
		true,
		EOnlineComparisonOp::Equals
	);

	SessionInterface->FindSessions
	(
		0,
		SessionSearch.ToSharedRef()
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

void UMultiplayerSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{

	if (SessionSearch.IsValid())
	{
		if (FindSessionsDoneDelegate.IsBound())
		{
			FindSessionsDoneDelegate.Broadcast(bWasSuccessful, SessionSearch->SearchResults);
		}
	}


}
