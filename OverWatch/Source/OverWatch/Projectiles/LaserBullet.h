// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/NormalProjectile.h"
#include "LaserBullet.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API ALaserBullet : public ANormalProjectile
{
	GENERATED_BODY()
	
protected:
	/** Particle System component to represent laser */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float BulletBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class UDamageType> BulletDamageType;

public:
	// Sets default values for this actor's properties
	ALaserBullet();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

protected:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
private:
	void PlayImpactEffectsBasedOnSurfaceType(EPhysicalSurface surfaceType, FVector Location);

public:
	void InitializeVelocity(FVector BulletVelocity);
};
