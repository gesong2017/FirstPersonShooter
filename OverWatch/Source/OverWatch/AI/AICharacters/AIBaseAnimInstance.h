// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "AIBaseAnimInstance.generated.h"

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

public:
	UAIBaseAnimInstance(const FObjectInitializer& _objectInitalizer);

protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void EnableDamageCollision();

	UFUNCTION(BlueprintCallable, Category = "Anim NotifyEvents")
	void DisableDamageCollision();
};
