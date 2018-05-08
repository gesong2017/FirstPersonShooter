// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MonsterMeleeAttackCombo.h"
#include "AI/AICharacters/BaseAIController.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "AI/AICharacters/LargeMonsterAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_MonsterMeleeAttackCombo::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
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

	const AActor* const TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIController->GetKeyID_TargetActor()));
	if (TargetActor == nullptr)
		return NodeResult;

	// Check Distance From Player
	float DistanceToTarget = BlackboardComp->GetValue<UBlackboardKeyType_Float>(AIController->GetKeyID_DistanceToTarget());

	// Too far to attack, fail the task
	if (DistanceToTarget > SwipingAttackDistance)
		return NodeResult;

	else
	{
		NodeResult = EBTNodeResult::InProgress;
		AIAnimInstance->OnFinishMeleeAttack.BindUObject(this, &UBTTask_MonsterMeleeAttackCombo::OnFinishAttackHandle);

		// Use different attack animation based on the distance
		if (DistanceToTarget <= SwipingAttackDistance && DistanceToTarget > PunchingAttackDistance)
			AIAnimInstance->MeleeAttack02();

		else
			AIAnimInstance->MeleeAttack01();

		return NodeResult;
	}
}

void UBTTask_MonsterMeleeAttackCombo::OnFinishAttackHandle(UBehaviorTreeComponent * OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	UE_LOG(LogTemp, Warning, TEXT("Monster Finished Melee Attack"))
}