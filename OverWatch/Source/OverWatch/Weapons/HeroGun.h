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
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
//	class USkeletalMeshComponent* FPPWeaponSkeletalMesh;
	
public:
	AHeroGun();

	//FORCEINLINE USkeletalMeshComponent* GetFirstPersonWeaponSkeletalMesh() const { return FPPWeaponSkeletalMesh; }

};
