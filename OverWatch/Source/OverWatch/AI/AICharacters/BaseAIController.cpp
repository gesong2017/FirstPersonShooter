// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include "BaseAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

ABaseAIController::ABaseAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
}

void ABaseAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// 
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(InPawn);
	UBehaviorTree* AIBehaviorTree = AICharacter->GetBehaviorTree();
	if (AICharacter && AIBehaviorTree)
	{   
		// Initialize AI Behavior Tree and blackboard
		BlackboardComp->InitializeBlackboard(*AIBehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*AIBehaviorTree);
	}

}


