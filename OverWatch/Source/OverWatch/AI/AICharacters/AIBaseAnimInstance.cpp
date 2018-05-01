// Fill out your copyright notice in the Description page of Project Settings.

#include "AIBaseAnimInstance.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AI/AICharacters/MeleeZombie.h"
#include "Components/BoxComponent.h"

UAIBaseAnimInstance::UAIBaseAnimInstance(const FObjectInitializer& _objectInitalizer) : Super(_objectInitalizer)
{
	// Intialize Variable
	Speed = 0.0f;
	bIsAlive = true;
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

void UAIBaseAnimInstance::EnableDamageCollision()
{
	AMeleeZombie* Zombie = Cast<AMeleeZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		UBoxComponent* CollisionBox = Zombie->GetCollisionBox();
		if (CollisionBox)
			CollisionBox->bGenerateOverlapEvents = true;
	}

}

void UAIBaseAnimInstance::DisableDamageCollision()
{
	AMeleeZombie* Zombie = Cast<AMeleeZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		UBoxComponent* CollisionBox = Zombie->GetCollisionBox();
		if (CollisionBox)
		{
			CollisionBox->bGenerateOverlapEvents = false;
			Zombie->ResetHitTimes();
		}
	}

}
