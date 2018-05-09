// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "LargeMonster.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API ALargeMonster : public ABaseAICharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Damage Info")
	class UBoxComponent* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Damage Info")
	class UBoxComponent* RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Damage Info")
	TSubclassOf<class UDamageType> MonsterDamageType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Damage Info")
	int HitTimes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Behavior Info")
	bool bIsTargetSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Behavior Info")
	TArray<AActor*> Stones;

public:
	// Sets default values for this character's properties
	ALargeMonster();

	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHand; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHand; }
	FORCEINLINE void ResetHitTimes() { HitTimes = 0; }
	FORCEINLINE TArray<AActor*> GetAllStones() const { return Stones; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	
protected:
	UFUNCTION()
	void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser) override;

	UFUNCTION()
	void OnTargetSensed(APawn* Pawn) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	AActor* GetClosestStoneByManhattanDistance();

	int GetIndexOfStone(AActor* TargetActor);

	void Pickup();

	void Throw();
};
