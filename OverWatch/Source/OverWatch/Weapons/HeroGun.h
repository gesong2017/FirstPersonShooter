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
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* FPPWeaponSkeletalMesh;

	//UPROPERTY(EditDefaultOnly, BlueprintReadOnly, Category = Weapon)
	//TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int MaxiumNumberOfBullets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int CurrentNumberOfBullets;

public:
	AHeroGun();

	void Fire();
	
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonWeaponSkeletalMesh() const { return FPPWeaponSkeletalMesh; }
	FORCEINLINE int32 GetMaxiumNumberOfBullets() const { return MaxiumNumberOfBullets; }
	FORCEINLINE int32 GetCurrentNumberOfBullets() const { return CurrentNumberOfBullets; }
	FORCEINLINE void UpdateCurrentNumberOfBullets(int NumOfBullets) { CurrentNumberOfBullets = NumOfBullets; }
};
