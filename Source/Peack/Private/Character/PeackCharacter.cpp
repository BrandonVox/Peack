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

#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"

#include "GameMode/PeackGameMode.h"




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
	// ECollisionChannel::ECC_GameTraceChannel1 = CustomCharacterMesh
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	// Spawn Method
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

// Called when the game starts or when spawned
void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShowLocalRole();

	if (HasAuthority())
	{
		SpawnWeapon();
		OnTakePointDamage.AddDynamic(this, &ThisClass::HandleTakePointDamage);
	}
}





// Server
void APeackCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Health = FMath::Max(Health - Damage, 0.0f);

	// server control this character
	if (IsLocallyControlled())
	{
		OnRep_Health();
	}

	if (Health > 0.0f)
	{
		Multicast_PlayHitReactMontage(ShotFromDirection);
	}
	else
	{
		if (APeackGameMode* PeackGameMode = GetAuthPeackGameMode())
		{
			PeackGameMode->HandleCharacterDead
			(
				InstigatedBy,
				GetController()
			);
		}

		// handle dead
		HandleDead();
	}
}



// Server
void APeackCharacter::HandleDead()
{
	// Disable Movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	Multicast_HandleDead();
	Client_HandleDead();

	SetLifeSpan(DeadSecond);
}

// Cient, Server
void APeackCharacter::Multicast_HandleDead_Implementation() // 
{
	// Multicast
// Mesh Component
// simulate phyics
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetPhysicsBlendWeight(1.0f);
	}

	// capsule component
	// disable
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APeackCharacter::Client_HandleDead_Implementation() // Implementation
{
	if (PeackPlayerController == nullptr)
	{
		PeackPlayerController = Cast<APeackPlayerController>(GetController());
	}

	DisableInput(PeackPlayerController);

	if (CameraComponent)
	{
		CameraComponent->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::KeepWorldTransform
		);
	}

}

// 100 -> 80
// 0 -> 0
// Client who controlled this character
void APeackCharacter::OnRep_Health()
{
	if (PeackPlayerController)
	{
		PeackPlayerController->UpdateBar_Health(Health, MaxHealth);
	}

}

void APeackCharacter::Multicast_PlayHitReactMontage_Implementation(const FVector& HitDirection)
{
	PlayAnimMontage(GetCorrectHitReactMontage(HitDirection));
}



UAnimMontage* APeackCharacter::GetCorrectHitReactMontage(const FVector& HitDirection) const
{
	double Dot = FVector::DotProduct(HitDirection, GetActorForwardVector());
	bool bShouldUseDot = FMath::Abs(Dot) > 0.5;

	if (bShouldUseDot)
	{
		if (Dot > 0.0)
		{
			return HitReactMontage_Back;
		}
		else
		{
			return HitReactMontage_Front;
		}
	}
	else
	{
		const FVector Cross = FVector::CrossProduct(HitDirection, GetActorForwardVector());
		if (Cross.Z > 0.0)
		{
			return HitReactMontage_Right;
		}
		else
		{
			return HitReactMontage_Left;
		}
	}

	return nullptr;
}

// Client, Server
void APeackCharacter::Destroyed()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}

	// request respawn
	if (HasAuthority())
	{
		RequestRespawn();
	}


	Super::Destroyed();
}



// Server
void APeackCharacter::RequestRespawn()
{
	if (UWorld* World = GetWorld())
	{
		APeackGameMode* PeackGameMode = World->GetAuthGameMode<APeackGameMode>();
		if (PeackGameMode)
		{
			PeackGameMode->RequestRespawn
			(
				this,
				GetController()
			);
		}
	}
}

APeackGameMode* APeackCharacter::GetAuthPeackGameMode() const
{
	if (UWorld* World = GetWorld())
	{
		return World->GetAuthGameMode<APeackGameMode>();
	}

	return nullptr;
}

// Server
// 3 Player Controller
// 1 Player Controller cua Server
void APeackCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Client_PlayerControllerReady();
}

// Local: Client, Server
void APeackCharacter::Client_PlayerControllerReady_Implementation()
{
	if (PeackPlayerController = Cast<APeackPlayerController>(GetController()))
	{
		PeackPlayerController->CreateWidget_Character();
		// Update Health Bar
		PeackPlayerController->UpdateBar_Health(Health, MaxHealth);
	}
}

void APeackCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APeackCharacter, CurrentWeapon);
	// tat ca client
	// dang dieu khien nhan vat
	// dieu kien
	DOREPLIFETIME_CONDITION(APeackCharacter, Health, COND_OwnerOnly);
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


// Server
void APeackCharacter::LineTraceFromCamera()
{
	if (CameraComponent == nullptr)
	{
		return;
	}

	const FVector& CameraLocation = CameraComponent->GetComponentLocation();
	const FVector& CameraDirection = CameraComponent->GetForwardVector();

	// distance camera <-> character
	const float TraceStartOffset = 
		FVector::Distance(CameraLocation, GetActorLocation()) + TraceHitOffset;

	const FVector StartLocation = CameraLocation + (CameraDirection * TraceStartOffset);
	const FVector EndLocation = StartLocation + (CameraDirection * 10'000'00);

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	const bool bDoHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		StartLocation,
		EndLocation,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	// Server
	if (bDoHit)
	{
		Multicast_SpawnHitEffect(HitResult.ImpactPoint);

		ApplyDamageToPeackCharacter(HitResult, CameraDirection);
	}
}

// Server
void APeackCharacter::ApplyDamageToPeackCharacter(const FHitResult& HitResult, const FVector& HitDirection)
{
	// gay sat thuong
	APeackCharacter* DamagedCharacter = Cast<APeackCharacter>(HitResult.GetActor());
	if (DamagedCharacter)
	{
		// apply point damage
		// gameplay statics
		UGameplayStatics::ApplyPointDamage(
			DamagedCharacter,
			20.0f,
			HitDirection,
			HitResult,
			GetController(),
			CurrentWeapon,
			UDamageType::StaticClass()
		);
	}
}

void APeackCharacter::Multicast_SpawnHitEffect_Implementation(const FVector& HitLocation) // Implementation
{
	// Hit Location
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitEffect,
		HitLocation
	);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		HitSound,
		HitLocation
	);
}

// Server
void APeackCharacter::Server_Fire_Implementation()
{
	if (bIsFiring)
	{
		return;
	}

	Multicast_Fire();
	LineTraceFromCamera();
	bIsFiring = true;

	FTimerManager& WorldTimerManager = GetWorldTimerManager();

	if (WorldTimerManager.IsTimerActive(FireDelayTimer))
	{
		WorldTimerManager.ClearTimer(FireDelayTimer);
	}

	WorldTimerManager.SetTimer(
		FireDelayTimer,
		this,
		&ThisClass::FireDelayFinished,
		1.0f / FireRate
	);
}



void APeackCharacter::FireDelayFinished()
{
	bIsFiring = false;
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