// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeackCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PEACK_API APeackCharacter : public ACharacter
{
	GENERATED_BODY()

public: // Function
	APeackCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private: // Property
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
};
