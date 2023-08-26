// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/PeackAnimInstance.h"

#include "Character/PeackCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UPeackAnimInstance::NativeInitializeAnimation()
{
	PeackCharacter = Cast<APeackCharacter>(GetOwningActor());
}

void UPeackAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (PeackCharacter == nullptr)
	{
		return;
	}

	Velocity = PeackCharacter->GetVelocity();

	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	/*
	* Strafing Value
	*/
	const FRotator& AimRotation = PeackCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Velocity);

	StrafingValue = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}
