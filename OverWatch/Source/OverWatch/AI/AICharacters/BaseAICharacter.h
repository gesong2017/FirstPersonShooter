// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class OVERWATCH_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	/** AI Behavior Tree */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior Tree")
	class UBehaviorTree* BehaviorTree;

	/** AI AttributeComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UAttributeComponent* AIAttributes;

	/** AI Sensing Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Perception)
	class UPawnSensingComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bIsAlive;

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE UAttributeComponent* GetAIAttributes() const { return AIAttributes; }
	FORCEINLINE bool IsBotAlive() const { return bIsAlive; }

protected:
	// NPC Get Killed
	void GetKilled();

	UFUNCTION()
	virtual void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser);

	UFUNCTION()
    void OnPawnSeen(APawn* Pawn);
};
