// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UBTService_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateDistanceToTarget();
	
protected:
	void TickNode(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory, float _deltaSecond) override;
	
};
