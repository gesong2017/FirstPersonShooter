// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

UCLASS()
class OVERWATCH_API AHero : public ACharacter
{
	GENERATED_BODY()

private:
	FTimerHandle AutomaticFireTimer;

	FTimerHandle StopReloadTimer;

	FTimerHandle ReloadGunTimer;

	float LastFireTime;

protected:
	/** 1st person camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FirstPersonCamera;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FirstPersonMesh;

	/** Hero AttributeComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UAttributeComponent* HeroAttributes;

	/** Hero gun classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AHeroGun* HeroGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AHeroGun> HeroGun_BP;

	/** Hero default bullets*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	int NumOfBulletsLeftOnHero;

	/** Hero state*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bCanFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsReloading;

public:
	// Sets default values for this character's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Functions for player basic movement
	void MoveForward(float Value);

	void MoveRight(float Value);

	// Functions for player fire and reload
	void StartFire();
	void StopFire();
	void FireOnce();
	void Reload();
	void StartReload();
	void StopReload();
	void ReloadGun();

	// Damage Processing
	UFUNCTION()
	void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser);

public:
	FRotator GetAimOffsets() const;
	
	void GetHeroCameraInformation(FVector &outPosition, FRotator &outRotation);

	float PlayReloadAnimMontage(float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	void  StopReloadAnimMontage();

	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	FORCEINLINE UAttributeComponent* GetHeroAttribute() const { return HeroAttributes;}
	FORCEINLINE bool IsHeroAlive() const { return bIsAlive; }
	FORCEINLINE void SetLastFireTime(float NewTime) { LastFireTime = NewTime; }
};
