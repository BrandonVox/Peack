// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeackCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UWidgetComponent;

class AWeapon;

UCLASS()
class PEACK_API APeackCharacter : public ACharacter
{
	GENERATED_BODY()

public: // Function
	virtual void PossessedBy(AController* NewController) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	APeackCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private: // Function

	void FireDelayFinished();

	UAnimMontage* GetCorrectHitReactMontage(const FVector& HitDirection) const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitReactMontage(const FVector& HitDirection);

	UFUNCTION()
	void HandleTakePointDamage(
		AActor* DamagedActor,
		float Damage,
		class AController* InstigatedBy,
		FVector HitLocation,
		class UPrimitiveComponent* FHitComponent,
		FName BoneName,
		FVector ShotFromDirection,
		const class UDamageType* DamageType,
		AActor* DamageCauser
	);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnHitEffect(const FVector& HitLocation);

	UFUNCTION(Client, Reliable)
	void Client_PlayerControllerReady();

	void LineTraceFromCamera();
	void ApplyDamageToPeackCharacter(const FHitResult& HitResult, const FVector& HitDirection);
	/*
	* Fire
	*/
	void Fire();

	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire();

	UFUNCTION()
	void OnRep_CurrentWeapon();

	void SpawnWeapon();
	void ShowLocalRole();

	void SetupInputMappingContext();
	/*
	* Input
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void FireButtonPressed();

private: // Property
	/*
	* Health
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;

	// Server
	bool bIsFiring = false;
	/*
	* Hit React
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Back;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Right;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Left;

	/*
	* Trace Hit
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TObjectPtr<UParticleSystem> HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float TraceHitOffset = 50.0f;

	/*
	* Fire
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UAnimMontage> FireMontage_Rifle;

	UPROPERTY()
	FTimerHandle FireDelayTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireRate = 2.0f;

	/*
	* Weapon
	*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	TObjectPtr<AWeapon> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName RifleSocketName;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	/*
	* Input
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Peack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Fire;
};
