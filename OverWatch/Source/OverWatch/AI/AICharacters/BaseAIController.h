// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

public:
	ABaseAIController();
	
	virtual void Possess(APawn* InPawn) override;

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
