// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PickupAndThrow.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UBTTask_PickupAndThrow : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	/** starts this task, should return Succeeded, Failed or InProgress
	*  (use FinishLatentTask() when returning InProgress)
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void OnFinishTaskHandle(UBehaviorTreeComponent* OwnerComp);
	
};
