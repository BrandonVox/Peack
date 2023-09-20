// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PeackPlayerState.generated.h"

class APeackPlayerController;

UCLASS()
class PEACK_API APeackPlayerState : public APlayerState
{
	GENERATED_BODY()

public: // Function

	void UpdateMVP(const bool bValue);

	UFUNCTION(Client, Reliable)
	void Client_UpdateMVP(const bool bValue);


	void SetDeath(const float NewDeath);

	float GetDeath() const
	{
		return Death;
	}




	APeackPlayerState();

	virtual void OnRep_Score() override;

	void AddOne_Score();
	void AddOne_Death();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private: // Function

	UFUNCTION()
	void OnRep_Death();

	void UpdateText_Score();
	void UpdateText_Death();


	UFUNCTION()
	void OnRep_Ready();

	bool IsLocallyControlled() const;

private: // Property

	UPROPERTY(ReplicatedUsing = OnRep_Death)
	float Death = 0.0f;

	UPROPERTY()
	TObjectPtr<APeackPlayerController> PeackPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bReady = false;
};
