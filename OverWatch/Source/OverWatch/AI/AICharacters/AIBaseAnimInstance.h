// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "AIBaseAnimInstance.generated.h"

DECLARE_DELEGATE_OneParam(FOnFinishMeleeAttackSignature, class UBehaviorTreeComponent*);
/**
 * 
 */
UCLASS()
class OVERWATCH_API UAIBaseAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Movement")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Montages")
	UAnimMontage* MeleeAttackMontage;

public:
	UAIBaseAnimInstance(const FObjectInitializer& _objectInitalizer);

	virtual void Attack();

	// Called When Attack Sequence is done by the end of animation sequence
	virtual void FinishMeleeAttack();

protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	virtual void UpdateAnimationProperties() override;

public:
	FOnFinishMeleeAttackSignature OnFinishMeleeAttack;
};
