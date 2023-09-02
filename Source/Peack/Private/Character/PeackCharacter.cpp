// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PeackCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/WidgetComponent.h"
#include "Widget/LocalRoleWidget.h"

#include "Weapon/Weapon.h"

#include "Net/UnrealNetwork.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Controller/PeackPlayerController.h"



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
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	// Z 90
	WidgetComponent->AddLocalOffset(FVector(0.0, 0.0, 90.0));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);

	// Movement Component
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// Set Object Type
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

// Server
// 3 Player Controller
// 1 Player Controller cua Server
void APeackCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Client_PlayerControllerReady();
}

void APeackCharacter::Client_PlayerControllerReady_Implementation()
{
	if (APeackPlayerController* PeackPlayerController = Cast<APeackPlayerController>(GetController()))
	{
		PeackPlayerController->CreateWidget_Character();
	}
}

void APeackCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APeackCharacter, CurrentWeapon);
}

// Called when the game starts or when spawned
void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShowLocalRole();

	if (HasAuthority())
	{
		SpawnWeapon();
	}
}





void APeackCharacter::SpawnWeapon()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;

	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);

	if (SpawnedWeapon)
	{
		FAttachmentTransformRules AttachmentRules = 
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		SpawnedWeapon->AttachToComponent(
			GetMesh(),
			AttachmentRules,
			RifleSocketName
		);
		// NULL -> !!!
		// Server -> Client
		CurrentWeapon = SpawnedWeapon;
	}
}




// Client
void APeackCharacter::OnRep_CurrentWeapon()
{
	// khi bien currentweapon thay doi
	// thi onrep se duoc goi
	// server tahy doi current weapon
	// thi server chuyen thong nay xuong cho cac client
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Purple,
			TEXT("OnRep_CurrentWeapon()")
		);
	}

}

void APeackCharacter::ShowLocalRole()
{
	if (WidgetComponent == nullptr)
	{
		return;
	}

	ULocalRoleWidget* LRW = Cast<ULocalRoleWidget>(WidgetComponent->GetUserWidgetObject());

	FText RoleText;

	switch (GetLocalRole())
	{
	case ENetRole::ROLE_Authority:
		RoleText = FText::FromString(TEXT("Authority"));
		break;
	case ENetRole::ROLE_AutonomousProxy:
		RoleText = FText::FromString(TEXT("Autonomous Proxy"));
		break;
	case ENetRole::ROLE_SimulatedProxy:
		RoleText = FText::FromString(TEXT("Simulated Proxy"));
		break;
	}

	if (LRW)
	{
		LRW->UpdateText_Role(RoleText);
	}
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

		EnhancedInputComponent->BindAction(
			IA_Fire,
			ETriggerEvent::Triggered,
			this,
			&ThisClass::FireButtonPressed
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

void APeackCharacter::FireButtonPressed()
{
	Fire();
}

// Client, Server, Local
void APeackCharacter::Fire()
{
	Server_Fire();
}



void APeackCharacter::LineTraceFromCamera()
{
	if (CameraComponent == nullptr)
	{
		return;
	}

	const FVector& CameraLocation = CameraComponent->GetComponentLocation();
	const FVector& CameraDirection = CameraComponent->GetForwardVector();
	const FVector EndLocation = CameraLocation + (CameraDirection * 10'000'00);

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		CameraLocation,
		EndLocation,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
}

// Server
void APeackCharacter::Server_Fire_Implementation()
{
	Multicast_Fire();
	LineTraceFromCamera();
}

// Client, Server
void APeackCharacter::Multicast_Fire_Implementation()
{
	if (CurrentWeapon)
	{
		PlayAnimMontage(FireMontage_Rifle);
		CurrentWeapon->WeaponFire();
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







