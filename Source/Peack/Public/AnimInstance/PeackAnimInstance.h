// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PeackAnimInstance.generated.h"

class APeackCharacter;
UCLASS()
class PEACK_API UPeackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float StrafingValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed = 0.0f;

private:
	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	TObjectPtr<APeackCharacter> PeackCharacter;
};
