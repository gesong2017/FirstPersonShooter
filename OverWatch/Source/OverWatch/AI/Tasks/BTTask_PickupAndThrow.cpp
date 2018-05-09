// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_PickupAndThrow.h"
#include "AI/AICharacters/BaseAIController.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "AI/AICharacters/LargeMonsterAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_PickupAndThrow::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	UBlackboardComponent* const BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
		return NodeResult;

	ABaseAIController* const AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return NodeResult;

	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(AIController->GetPawn());
	if (AICharacter == nullptr)
		return NodeResult;

	ULargeMonsterAnimInstance* AIAnimInstance = Cast<ULargeMonsterAnimInstance>(AICharacter->GetMesh()->GetAnimInstance());
	if (AIAnimInstance == nullptr)
		return NodeResult;

	NodeResult = EBTNodeResult::InProgress;
	AIAnimInstance->OnFinishMeleeAttack.BindUObject(this, &UBTTask_PickupAndThrow::OnFinishTaskHandle);
	AIAnimInstance->RangeAttack();

	return NodeResult;
}

void UBTTask_PickupAndThrow::OnFinishTaskHandle(UBehaviorTreeComponent * OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	UE_LOG(LogTemp, Warning, TEXT("Monster Finished Range Attack"))
}

