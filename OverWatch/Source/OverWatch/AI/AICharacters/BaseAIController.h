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
	// Blackboard variable keyid
	uint8 TargetKeyID;
	uint8 DistanceSqrToTargetKeyID;
	uint8 DistanceToTargetKeyID;
	uint8 ClosestPickupKeyID;
	uint8 StartLocationKeyID;
	FName StartLocationKeyName;
	FName PatrolLocationKeyName;

protected:
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComp;
	
public:
	ABaseAIController();

	void Possess(APawn* InPawn) override;

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() const { return BehaviorTreeComp; }

	// Get Key ID of variable in blackboard
	FORCEINLINE uint8 GetKeyID_TargetActor() const { return TargetKeyID; }
	FORCEINLINE uint8 GetKeyID_DistanceSqrToTarget() const { return DistanceSqrToTargetKeyID; }
	FORCEINLINE uint8 GetKeyID_DistanceToTarget() const { return DistanceToTargetKeyID; }
	FORCEINLINE uint8 GetKeyID_ClosestPickup() const { return ClosestPickupKeyID; }
	FORCEINLINE uint8 GetKeyID_StartLocation() const { return StartLocationKeyID; }

protected:
	UFUNCTION(BlueprintCallable)
	FName GetStartLocationKeyName() const;

	UFUNCTION(BlueprintCallable)
	FName GetPatrolLocationKeyName() const;
};
