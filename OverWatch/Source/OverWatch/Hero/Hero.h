// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

UCLASS()
class OVERWATCH_API AHero : public ACharacter
{
	GENERATED_BODY()

protected:
	/** 1st person camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCamera;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	/** Hero gun classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AHeroGun* HeroGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AHeroGun> HeroGun_BP;

	/** Hero default bullets*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int NumOfBulletsLeftOnHero;

public:
	// Sets default values for this character's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Functions for player basic movement
	void MoveForward(float Value);

	void MoveRight(float Value);

	// Functions for player fire and reload
	void StartFire();
	void StopFire();
	void Reload();

public:
	FRotator GetAimOffsets() const;
	
	void GetHeroCameraInformation(FVector &outPosition, FRotator &outRotation);
};
