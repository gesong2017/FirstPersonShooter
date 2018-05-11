// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalRangeTrooperAnimInstance.h"
#include "AI/AICharacters/NormalRangeTrooper.h"
#include "AI/AICharacters/BaseAIController.h"
#include "Kismet/KismetMathLibrary.h"

UNormalRangeTrooperAnimInstance::UNormalRangeTrooperAnimInstance(const FObjectInitializer & _objectInitalizer) : Super(_objectInitalizer)
{
	// Intialize Variable
	Direction = 0.0f;
	AimAngle = 0.0f;
}

void UNormalRangeTrooperAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		ANormalRangeTrooper* NPC = Cast<ANormalRangeTrooper>(Pawn);
		if (NPC)
		{
			// Set AI Speed
			Speed = NPC->GetVelocity().Size();
			if (Speed <= 0.0f)
			{
				Speed = 0.0f;
				Direction = 0.0f;
			}
			else
			{
				// Set AI Direction
				FRotator RotationFromWorldX = UKismetMathLibrary::MakeRotFromX(NPC->GetVelocity());
				FRotator NPCRotation = NPC->GetActorRotation().GetInverse();
				FRotator TranslatedRotator = UKismetMathLibrary::ComposeRotators(RotationFromWorldX, NPCRotation);
				if (TranslatedRotator.Yaw >= 180.0f)
					Direction = TranslatedRotator.Yaw - 360.0f;
				else
					Direction = TranslatedRotator.Yaw;
			}

			// Update Aim Angle
			AimAngle = NPC->GetControlRotation().Pitch;

			// Set AI Alive state
			bIsAlive = NPC->IsBotAlive();
		}
	}
}

void UNormalRangeTrooperAnimInstance::Fire()
{
	if (FireMontage)
	{
		Montage_Play(FireMontage);
		UE_LOG(LogTemp, Warning, TEXT("Fire Montage is playing"))
	}
}

void UNormalRangeTrooperAnimInstance::FinishMeleeAttack()
{
	//Get the pawn which is affected by our anim instance
	ANormalRangeTrooper* NormalRangeTrooper = Cast<ANormalRangeTrooper>(TryGetPawnOwner());
	if (NormalRangeTrooper)
	{
		ABaseAIController* BaseAIController = Cast<ABaseAIController>(NormalRangeTrooper->GetController());
		if (BaseAIController)
		{   
			UBehaviorTreeComponent* BehaviorTreeComp = BaseAIController->GetBehaviorTreeComp();
			if (BehaviorTreeComp)
			{   
				OnFinishMeleeAttack.ExecuteIfBound(BehaviorTreeComp);
				UE_LOG(LogTemp, Warning, TEXT("Finish Fire Got Called"))
			}
		}
	}
}
