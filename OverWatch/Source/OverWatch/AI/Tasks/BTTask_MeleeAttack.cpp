// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MeleeAttack.h"
#include "AI/AICharacters/BaseAIController.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "AI/AICharacters/AIBaseAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
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

	UAIBaseAnimInstance* AIAnimInstance = Cast<UAIBaseAnimInstance>(AICharacter->GetMesh()->GetAnimInstance());
	if (AIAnimInstance == nullptr)
		return NodeResult;

	const AActor* const TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIController->GetKeyID_TargetActor()));
	if (TargetActor == nullptr)
		return NodeResult;

	// check attack distance
	float DistanceSqr = (AICharacter->GetActorLocation() - TargetActor->GetActorLocation()).SizeSquared();
	BlackboardComp->SetValue<UBlackboardKeyType_Float>(AIController->GetKeyID_DistanceSqrToTarget(), DistanceSqr);

	// Too far to attack, fail the task
	if (DistanceSqr > MeleeAttackDistanceSqr)
		return NodeResult;

	else
	{   
		NodeResult = EBTNodeResult::InProgress;
		AIAnimInstance->OnFinishMeleeAttack.BindUObject(this, &UBTTask_MeleeAttack::OnFinishAttackHandle);
		AIAnimInstance->Attack();
		return NodeResult;
	}

}

void UBTTask_MeleeAttack::OnFinishAttackHandle(UBehaviorTreeComponent * OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	UE_LOG(LogTemp, Warning, TEXT("Zombie finish Attacking"))
}
