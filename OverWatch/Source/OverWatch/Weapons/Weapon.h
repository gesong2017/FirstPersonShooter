// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class OVERWATCH_API AWeapon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* WeaponSkeletalMesh;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

public:	
	FORCEINLINE USkeletalMeshComponent* GetWeaponSkeletalMesh() const { return WeaponSkeletalMesh; }
};
