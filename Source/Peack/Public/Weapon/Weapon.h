
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PEACK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	// Function
	AWeapon();
	// virtual void Tick(float DeltaTime) override;

	void WeaponFire();

protected:
	virtual void BeginPlay() override;

private: // Property

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UAnimationAsset> AA_Fire;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

};
