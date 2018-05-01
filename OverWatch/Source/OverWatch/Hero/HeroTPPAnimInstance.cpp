// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroTPPAnimInstance.h"
#include "Hero.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

UHeroTPPAnimInstance::UHeroTPPAnimInstance(const FObjectInitializer& _objectInitalizer) : Super(_objectInitalizer)
{
	// Intialize Variable
	Speed = 0.0f;
	Direction = 0.0f;
	bIsJumping = false;
	bIsAlive = true;
	Yaw = 0.0f;
	Pitch = 0.0f;
}

void UHeroTPPAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		AHero* Hero = Cast<AHero>(Pawn);
		if (Hero)
		{   
			// Set Hero Speed
			Speed = Hero->GetVelocity().Size();
			if (Speed <= 0.0f)
			{
				Speed = 0.0f;
				Direction = 0.0f;
			}
			else
			{
				// Set Hero Direction
				FRotator RotationFromWorldX = UKismetMathLibrary::MakeRotFromX(Hero->GetVelocity());
				FRotator HeroRotation = Hero->GetActorRotation().GetInverse();
				FRotator TranslatedRotator = UKismetMathLibrary::ComposeRotators(RotationFromWorldX, HeroRotation);
				if (TranslatedRotator.Yaw >= 180.0f)
					Direction = TranslatedRotator.Yaw - 360.0f;
				else
					Direction = TranslatedRotator.Yaw;
			}

			// Set Up Jump State
			bIsJumping = Hero->GetMovementComponent()->IsFalling();

			// Set AimOffsets variable
			Yaw = Hero->GetAimOffsets().Yaw;
			Pitch = Hero->GetAimOffsets().Pitch;

			// Set Player Alive
			bIsAlive = Hero->IsHeroAlive();
		}
	}
}


