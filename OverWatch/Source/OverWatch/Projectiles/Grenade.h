// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/NormalProjectile.h"
#include "Grenade.generated.h"

UENUM(BlueprintType)
enum class EGrenadeType : uint8
{
	NormalGrenade 		     UMETA(DisplayName = "NormalGrenade"),
	SmokeGrenade			 UMETA(DisplayName = "SmokeGrenade")
};

/**
 * 
 */
UCLASS()
class OVERWATCH_API AGrenade : public ANormalProjectile
{
	GENERATED_BODY()

protected:
	/** Particle System component to represent laser */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	EGrenadeType GrenadeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float GrenadeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class UDamageType> GrenadeDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int HitTimes;

public:
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

protected:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	void InitializeVelocity(FVector BulletVelocity);
};
