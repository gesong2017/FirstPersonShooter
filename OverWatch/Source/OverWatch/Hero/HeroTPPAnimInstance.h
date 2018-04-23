// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "HeroTPPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UHeroTPPAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Movement")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Movement")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimOffsets")
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AimOffsets")
	float Pitch;
	
public:
	UHeroTPPAnimInstance(const FObjectInitializer& _objectInitalizer);
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;
};
