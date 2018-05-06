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
	/** Pawn Sensing Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Sensing")
	class UPawnSensingComponent* PawnSensing;

	/** AI AttributeComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Attributes")
	class UAttributeComponent* AIAttributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI State")
	bool bIsAlive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Behavior")
	class UBehaviorTree* BotBehaviorTree;

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FORCEINLINE UAttributeComponent* GetAIAttributes() const { return AIAttributes; }
	FORCEINLINE bool IsBotAlive() const { return bIsAlive; }
	FORCEINLINE UBehaviorTree* GetBotBehaviorTree() const { return BotBehaviorTree; }

protected:
	// NPC Get Killed
	void GetKilled();

	UFUNCTION()
	virtual void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnTargetSensed(APawn* Pawn);
};
