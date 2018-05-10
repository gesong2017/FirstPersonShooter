// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Fire.h"
#include "AI/AICharacters/BaseAIController.h"
#include "AI/AICharacters/NormalRangeTrooper.h"
#include "AI/AICharacters/NormalRangeTrooperAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	UBlackboardComponent* const BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
		return NodeResult;

	ABaseAIController* const AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return NodeResult;

	ANormalRangeTrooper* NormalRangeTrooper = Cast<ANormalRangeTrooper>(AIController->GetPawn());
	if (NormalRangeTrooper == nullptr)
		return NodeResult;

	UNormalRangeTrooperAnimInstance* NormalRangeTrooperAnimInstance = Cast<UNormalRangeTrooperAnimInstance>(NormalRangeTrooper->GetMesh()->GetAnimInstance());
	if (NormalRangeTrooperAnimInstance == nullptr)
		return NodeResult;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIController->GetKeyID_TargetActor()));
	if (TargetActor == nullptr)
		return NodeResult;

	// Fire!
	NodeResult = EBTNodeResult::InProgress;
	NormalRangeTrooperAnimInstance->OnFinishMeleeAttack.BindUObject(this, &UBTTask_Fire::OnFinishFireHandle);
	NormalRangeTrooperAnimInstance->Fire();
	NormalRangeTrooper->Fire(TargetActor);

	return NodeResult;
}

void UBTTask_Fire::OnFinishFireHandle(UBehaviorTreeComponent * OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
