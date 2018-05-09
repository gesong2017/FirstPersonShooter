// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindClosestStone.h"
#include "AI/AICharacters/BaseAIController.h"
#include "AI/AICharacters/LargeMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


EBTNodeResult::Type UBTTask_FindClosestStone::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	UBlackboardComponent* const BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
		return NodeResult;

	ABaseAIController* const AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return NodeResult;

	ALargeMonster* LargeMonster = Cast<ALargeMonster>(AIController->GetPawn());
	if (LargeMonster == nullptr)
		return NodeResult;

	TArray<AActor*> AvailablePickup = LargeMonster->GetAllStones();
	if (AvailablePickup.Num() == 0)
		return NodeResult;

	// Get the closest stone near monster'
	AActor* ClosestProjectile = LargeMonster->GetClosestStoneByManhattanDistance();
	if (ClosestProjectile == nullptr)
		return NodeResult;

	// Update blackboard value
	BlackboardComp->SetValue<UBlackboardKeyType_Object>(AIController->GetKeyID_ClosestPickup(), ClosestProjectile);
	NodeResult = EBTNodeResult::Succeeded;
	return NodeResult;
}
