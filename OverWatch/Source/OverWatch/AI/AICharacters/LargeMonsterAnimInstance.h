// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/AIBaseAnimInstance.h"
#include "LargeMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API ULargeMonsterAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Montages")
	UAnimMontage* MeleeAttackMontage_01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Montages")
	UAnimMontage* MeleeAttackMontage_02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Montages")
	UAnimMontage* RangeAttackMontage;

public:
	ULargeMonsterAnimInstance(const FObjectInitializer& _objectInitalizer);
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void EnableLeftHandDamageCollision();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void DisableLeftHandDamageCollision();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void EnableRightHandDamageCollision();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void DisableRightHandDamageCollision();
	
public:
	void MeleeAttack01();

	void MeleeAttack02();

	void RangeAttack();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void FinishMeleeAttack() override;
};
