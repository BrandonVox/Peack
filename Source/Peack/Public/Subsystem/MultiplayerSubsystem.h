// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCreateSessionDoneDelegate, bool bWasSuccessful);

DECLARE_MULTICAST_DELEGATE_OneParam(FDestroySessionDoneDelegate, bool bWasSuccessful);

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FFindSessionsDoneDelegate,
	bool bWasSuccessful,
	const TArray<FOnlineSessionSearchResult>& SearchResults
);

DECLARE_MULTICAST_DELEGATE_OneParam(FJoinSessionDoneDelegate, bool bWasSuccessful);


UCLASS()
class PEACK_API UMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public: // Function

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void CreateSession();

	void FindSessions();

	// Search result 
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);


private: // Function
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);
	
public: // Property
	FCreateSessionDoneDelegate CreateSessionDoneDelegate;
	FDestroySessionDoneDelegate DestroySessionDoneDelegate;

	FFindSessionsDoneDelegate FindSessionsDoneDelegate;

	FJoinSessionDoneDelegate JoinSessionDoneDelegate;

private: // Property
	IOnlineSessionPtr SessionInterface;


	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// Listen Server
	FString Path_FirstMap = TEXT("/Game/Map/FirstMap?listen");
};
