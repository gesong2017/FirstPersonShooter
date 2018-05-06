// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeZombieAnimInstance.h"
#include "AI/AICharacters/MeleeZombie.h"
#include "Components/BoxComponent.h"
#include "AI/AICharacters/BaseAIController.h"

UMeleeZombieAnimInstance::UMeleeZombieAnimInstance(const FObjectInitializer& _objectInitalizer) : Super(_objectInitalizer)
{

}

void UMeleeZombieAnimInstance::UpdateAnimationProperties()
{
	Super::UpdateAnimationProperties();
}

void UMeleeZombieAnimInstance::EnableDamageCollision()
{
	AMeleeZombie* Zombie = Cast<AMeleeZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		UBoxComponent* CollisionBox = Zombie->GetCollisionBox();
		if (CollisionBox)
			CollisionBox->bGenerateOverlapEvents = true;
	}
}

void UMeleeZombieAnimInstance::DisableDamageCollision()
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

void UMeleeZombieAnimInstance::Attack()
{   
	if (MeleeAttackMontage)
	{   
		Montage_Play(MeleeAttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("Zombie is Attacking"))
	}
}

void UMeleeZombieAnimInstance::FinishMeleeAttack()
{   
	//Get the pawn which is affected by our anim instance
	AMeleeZombie* Zombie = Cast<AMeleeZombie>(TryGetPawnOwner());
	if (Zombie)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(Zombie->GetController());
		if (BaseAIController)
		{
			UBehaviorTreeComponent* BehaviorTreeComp = BaseAIController->GetBehaviorTreeComp();
			if (BehaviorTreeComp)
				OnFinishMeleeAttack.ExecuteIfBound(BehaviorTreeComp);
		}
	}
}
