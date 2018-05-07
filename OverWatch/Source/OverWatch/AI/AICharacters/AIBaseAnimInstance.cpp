// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBaseAnimInstance.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "GameFramework/PawnMovementComponent.h"

UAIBaseAnimInstance::UAIBaseAnimInstance(const FObjectInitializer& _objectInitalizer) : Super(_objectInitalizer)
{
	// Intialize Variable
	Speed = 0.0f;
	bIsAlive = true;
}

void UAIBaseAnimInstance::FinishMeleeAttack()
{
}

void UAIBaseAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		ABaseAICharacter* NPC = Cast<ABaseAICharacter>(Pawn);
		if (NPC)
		{
			// Set AI Speed
			Speed = NPC->GetVelocity().Size();

			// Set AI Alive state
			bIsAlive = NPC->IsBotAlive();
		}
	}
}

