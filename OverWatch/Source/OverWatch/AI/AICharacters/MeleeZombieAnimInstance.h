// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/AIBaseAnimInstance.h"
#include "MeleeZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UMeleeZombieAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

public:
	UMeleeZombieAnimInstance(const FObjectInitializer& _objectInitalizer);

protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void EnableDamageCollision();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void DisableDamageCollision();

public:
	void Attack() override;

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void FinishMeleeAttack() override;
};
