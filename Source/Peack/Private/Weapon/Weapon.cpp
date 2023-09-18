// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	// Mesh Component
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(MeshComponent);

	// Cosmetic
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::WeaponFire()
{
	if (MeshComponent)
	{
		MeshComponent->PlayAnimation(AA_Fire, false);
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

