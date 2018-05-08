// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include "BaseAICharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

ABaseAIController::ABaseAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void ABaseAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// cast possessed pawn to base ai character
	ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(InPawn);
	UBehaviorTree* BotBehaviorTree = AICharacter->GetBotBehaviorTree();
	if (AICharacter && BotBehaviorTree)
	{
		// Initialize Blackboard component
		BlackboardComp->InitializeBlackboard(*BotBehaviorTree->BlackboardAsset);

		// store variable in BB to our keyid
		TargetKeyID = BlackboardComp->GetKeyID(TEXT("Target"));
		DistanceSqrToTargetKeyID= BlackboardComp->GetKeyID(TEXT("DistanceSqrToTarget"));
		DistanceToTargetKeyID = BlackboardComp->GetKeyID(TEXT("DistanceToTarget"));

		// Initialize Distance To Avoid Entering into Wrong Condition Check
		BlackboardComp->SetValue<UBlackboardKeyType_Float>(DistanceToTargetKeyID, 10000.0f);

		// Run Behavior Tree
		BehaviorTreeComp->StartTree(*BotBehaviorTree);
	}
}


