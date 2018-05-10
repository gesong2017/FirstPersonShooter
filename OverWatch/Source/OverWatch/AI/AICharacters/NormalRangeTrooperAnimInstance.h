// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/AIBaseAnimInstance.h"
#include "NormalRangeTrooperAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UNormalRangeTrooperAnimInstance : public UAIBaseAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Movement")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim Montages")
	UAnimMontage* FireMontage;

public:
	UNormalRangeTrooperAnimInstance(const FObjectInitializer& _objectInitalizer);

protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;

public:
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void FinishMeleeAttack() override;
};
