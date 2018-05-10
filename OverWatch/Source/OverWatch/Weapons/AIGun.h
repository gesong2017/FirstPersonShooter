// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "AIGun.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AAIGun : public AWeapon
{
	GENERATED_BODY()

private:
	float DivByBulletSpeed;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	FName MuzzleSocketName;

	/** Laser Bullet Blueprint classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class ALaserBullet> LaserBullet_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float BulletSpeed;

public:
	AAIGun();

protected:
	void BeginPlay() override;

public:
	void Fire(AActor* Target);
	
};
