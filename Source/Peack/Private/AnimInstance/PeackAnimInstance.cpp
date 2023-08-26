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

	GroundSpeed = UKismetMathLibrary::VSizeXY(PeackCharacter->GetVelocity());
}
