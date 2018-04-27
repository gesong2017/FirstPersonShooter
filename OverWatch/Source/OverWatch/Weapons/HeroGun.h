// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "HeroGun.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AHeroGun : public AWeapon
{
	GENERATED_BODY()
private:
	FTimerHandle AutomaticFireTimer;

	float LastFireTime;

	float TimeBetweenShots;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* FPPWeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<class UCameraShake> FireRecoil;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int MaxiumNumberOfBullets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int CurrentNumberOfBullets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	int GunBaseDamage;

	// Bullets Per Minute Fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float GunFireRate;

public:
	AHeroGun();

protected:
	virtual void BeginPlay() override;

public:
	void StartFire();
	void StopFire();

	FORCEINLINE USkeletalMeshComponent* GetFirstPersonWeaponSkeletalMesh() const { return FPPWeaponSkeletalMesh; }
	FORCEINLINE int GetMaxiumNumberOfBullets() const { return MaxiumNumberOfBullets; }
	FORCEINLINE int GetCurrentNumberOfBullets() const { return CurrentNumberOfBullets; }
	FORCEINLINE void UpdateCurrentNumberOfBullets(int NumOfBullets) { CurrentNumberOfBullets = NumOfBullets; }
	FORCEINLINE int GetGunBaseDamage() const { return GunBaseDamage; }

protected:
	void FireOnce();

private:
	void PlayImpactEffectsBasedOnSurfaceType(EPhysicalSurface surfaceType, FVector Location);
};
