// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "AIGun.generated.h"

UENUM(BlueprintType)
enum class EAIGunType : uint8
{
	Rifle 				     UMETA(DisplayName = "Rifle"),
	GrenadeLanucher			 UMETA(DisplayName = "GrenadeLanucher")
};

/**
 * 
 */
UCLASS()
class OVERWATCH_API AAIGun : public AWeapon
{
	GENERATED_BODY()

private:
	float DivByBulletSpeed;

	FTimerHandle SmokeGrenadeCoolDown;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EAIGunType GunType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	FName MuzzleSocketName;

	/** Laser Bullet Blueprint classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class ALaserBullet> LaserBullet_BP;

	/** Grenade Blueprint classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class AGrenade> NormalGrenade_BP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class AGrenade> SmokeGrenade_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float BulletSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	bool bCanFireSmokeGrenade;

public:
	AAIGun();

protected:
	void BeginPlay() override;

public:
	void Fire(AActor* Target);

private:
	void RifleFire(AActor* Target);
	
	void GrenadeFire(AActor* Target);

	FORCEINLINE void EnableSmokeGrenade() { bCanFireSmokeGrenade = true; }
};
