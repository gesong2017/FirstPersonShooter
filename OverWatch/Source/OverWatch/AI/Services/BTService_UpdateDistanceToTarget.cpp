// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_UpdateDistanceToTarget.h"
#include "AI/AICharacters/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTService_UpdateDistanceToTarget::UBTService_UpdateDistanceToTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent & _ownerComp, uint8 * _nodeMemory, float _deltaSecond)
{   
	UBlackboardComponent* BlackboardComp = _ownerComp.GetBlackboardComponent();
	ABaseAIController* AIController = Cast<ABaseAIController>(_ownerComp.GetAIOwner());
	if (BlackboardComp && AIController)
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(AIController->GetKeyID_TargetActor()));
		APawn* AICharacter = AIController->GetPawn();
		if (TargetActor && AICharacter)
		{
			float DistanceToTarget = (AICharacter->GetActorLocation() - TargetActor->GetActorLocation()).Size();
			BlackboardComp->SetValue<UBlackboardKeyType_Float>(AIController->GetKeyID_DistanceToTarget(), DistanceToTarget);
		}
	}
}
