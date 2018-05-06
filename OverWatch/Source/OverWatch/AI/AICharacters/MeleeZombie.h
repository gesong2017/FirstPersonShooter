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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Damage Info")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Damage Info")
	TSubclassOf<class UDamageType> ZombieDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Damage Info")
	int HitTimes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Behavior Info")
	bool bIsTargetSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Behavior Info")
	float MaxiumSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Behavior Info")
	float NormalSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Behavior Info")
	int TimesHitByHero;

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
	void OnTargetSensed(APawn* Pawn);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
