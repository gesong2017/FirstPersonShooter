// Fill out your copyright notice in the Description page of Project Settings.

#include "LargeMonsterAnimInstance.h"
#include "AI/AICharacters/LargeMonster.h"
#include "Components/BoxComponent.h"
#include "AI/AICharacters/BaseAIController.h"

ULargeMonsterAnimInstance::ULargeMonsterAnimInstance(const FObjectInitializer & _objectInitalizer) : Super(_objectInitalizer)
{
}

void ULargeMonsterAnimInstance::UpdateAnimationProperties()
{
	Super::UpdateAnimationProperties();
}

void ULargeMonsterAnimInstance::EnableLeftHandDamageCollision()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
	{
		UBoxComponent* LeftHandCollisionBox = LargeMonster->GetLeftHandCollisionBox();
		if (LeftHandCollisionBox)
			LeftHandCollisionBox->bGenerateOverlapEvents = true;
	}
}

void ULargeMonsterAnimInstance::DisableLeftHandDamageCollision()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
	{
		UBoxComponent* LeftHandCollisionBox = LargeMonster->GetLeftHandCollisionBox();
		if (LeftHandCollisionBox)
		{
			LeftHandCollisionBox->bGenerateOverlapEvents = false;
			LargeMonster->ResetHitTimes();
		}
	}
}

void ULargeMonsterAnimInstance::EnableRightHandDamageCollision()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
	{
		UBoxComponent* RightHandCollisionBox = LargeMonster->GetRightHandCollisionBox();
		if (RightHandCollisionBox)
			RightHandCollisionBox->bGenerateOverlapEvents = true;
	}
}

void ULargeMonsterAnimInstance::DisableRightHandDamageCollision()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
	{
		UBoxComponent* RightHandCollisionBox = LargeMonster->GetRightHandCollisionBox();
		if (RightHandCollisionBox)
		{
			RightHandCollisionBox->bGenerateOverlapEvents = false;
			LargeMonster->ResetHitTimes();
		}
	}
}

void ULargeMonsterAnimInstance::GrabStone()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
		LargeMonster->Pickup();
}

void ULargeMonsterAnimInstance::ThrowStone()
{
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
		LargeMonster->Throw();
}

void ULargeMonsterAnimInstance::MeleeAttack01()
{
	if (MeleeAttackMontage_01)
	{
		Montage_Play(MeleeAttackMontage_01);
		UE_LOG(LogTemp, Warning, TEXT("Monster is Punching"))
	}
}

void ULargeMonsterAnimInstance::MeleeAttack02()
{
	if (MeleeAttackMontage_02)
	{
		Montage_Play(MeleeAttackMontage_02);
		UE_LOG(LogTemp, Warning, TEXT("Monster is Swiping"))
	}
}

void ULargeMonsterAnimInstance::RangeAttack()
{
	if (RangeAttackMontage)
	{
		Montage_Play(RangeAttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("Monster is range attacking"))
	}
}

void ULargeMonsterAnimInstance::FinishMeleeAttack()
{
	//Get the pawn which is affected by our anim instance
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(TryGetPawnOwner());
	if (LargeMonster)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(LargeMonster->GetController());
		if (BaseAIController)
		{
			UBehaviorTreeComponent* BehaviorTreeComp = BaseAIController->GetBehaviorTreeComp();
			if (BehaviorTreeComp)
				OnFinishMeleeAttack.ExecuteIfBound(BehaviorTreeComp);
		}
	}
}
