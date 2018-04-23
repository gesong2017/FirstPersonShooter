// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "HeroFPPAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UHeroFPPAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Movement")
	bool bIsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hero Movement")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim")
	bool bIsAiming;
	
public:
	UHeroFPPAnimInstance(const FObjectInitializer& _objectInitalizer);

protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties() override;
	
};
