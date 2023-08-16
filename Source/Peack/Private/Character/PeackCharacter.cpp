// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PeackCharacter.h"

// Sets default values
APeackCharacter::APeackCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust Mesh Component
	GetMesh()->AddLocalOffset(FVector(0.0, 0.0, -88.0));
	GetMesh()->AddLocalRotation(FRotator(0.0, -90.0, 0.0));
}

// Called when the game starts or when spawned
void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APeackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

