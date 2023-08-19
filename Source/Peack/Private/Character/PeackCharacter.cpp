// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PeackCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APeackCharacter::APeackCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust Mesh Component
	GetMesh()->AddLocalOffset(FVector(0.0, 0.0, -88.0));
	GetMesh()->AddLocalRotation(FRotator(0.0, -90.0, 0.0));

	// Spring Arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Net
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 100.0f;

	// Widget Component
}

// Called when the game starts or when spawned
void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();

	ENetRole::ROLE_Authority;
	ENetRole::ROLE_AutonomousProxy;
	ENetRole::ROLE_SimulatedProxy;
}

void APeackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupInputMappingContext();

	// Setup Input Action
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			IA_Look,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::Look
		);

		EnhancedInputComponent->BindAction(
			IA_Move,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::Move
		);
	}
}

void APeackCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Value_Vector2D = Value.Get<FVector2D>();

	const FRotator ControllerRotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);

	if (Value_Vector2D.Y != 0.0)
	{
		const FVector ForwardDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, Value_Vector2D.Y);
	}

	if (Value_Vector2D.X != 0.0)
	{
		const FVector RightDirection = ControllerRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, Value_Vector2D.X);
	}
}

void APeackCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Value_Vector2D = Value.Get<FVector2D>();

	if (Value_Vector2D.X != 0.0)
	{
		AddControllerYawInput(Value_Vector2D.X);
	}

	if (Value_Vector2D.Y != 0.0)
	{
		AddControllerPitchInput(Value_Vector2D.Y);
	}
}

void APeackCharacter::SetupInputMappingContext()
{
	if ( APlayerController* MyPlayerController = Cast<APlayerController>(GetController()) )
	{
		ULocalPlayer* MyLocalPlayer = MyPlayerController->GetLocalPlayer();

		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyLocalPlayer);

		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_Peack, 0);
		}
	}
}







