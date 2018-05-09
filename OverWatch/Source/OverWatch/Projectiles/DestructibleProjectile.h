// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleProjectile.generated.h"

UCLASS()
class OVERWATCH_API ADestructibleProjectile : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UDestructibleComponent* DestructibleComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	bool bIsDestructible;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	bool bCanDamageHero;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float ProjectileDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TSubclassOf<class UDamageType> ProjectileDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int HitTimes;

public:	
	// Sets default values for this actor's properties
	ADestructibleProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	FORCEINLINE UDestructibleComponent* GetDestructibleComp() const { return DestructibleComp; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComp() const { return ProjectileMovementComp; }
	FORCEINLINE bool GetIsDestructible() const { return bIsDestructible; }

protected:
	UFUNCTION()
	void OnHitHero(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnMeshDestructed(const FVector &HitPoint, const FVector &HitDirection);

	UFUNCTION()
	void OnFinishFlying(const FHitResult& ImpactResult);

public:
	void IntializeProjectile(AActor* TargetActor);
};
