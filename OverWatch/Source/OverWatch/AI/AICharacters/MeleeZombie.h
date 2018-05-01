// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "MeleeZombie.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AMeleeZombie : public ABaseAICharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionInfo")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CollisionInfo")
	TSubclassOf<class UDamageType> ZombieDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionInfo")
	int HitTimes;

public:
	// Sets default values for this character's properties
	AMeleeZombie();

	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE void ResetHitTimes() { HitTimes = 0; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	
protected:
	UFUNCTION()
	void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser) override;

	UFUNCTION()
	void OnCollisionBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
