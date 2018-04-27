// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroFPPAnimInstance.h"
#include "Hero.h"
#include "GameFramework/PawnMovementComponent.h"

UHeroFPPAnimInstance::UHeroFPPAnimInstance(const FObjectInitializer& _objectInitalizer) : Super(_objectInitalizer)
{
	bIsWalking = false;
	bIsJumping = false;
	bIsAiming = false;
}

void UHeroFPPAnimInstance::UpdateAnimationProperties()
{
	//Get the pawn which is affected by our anim instance
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		AHero* Hero = Cast<AHero>(Pawn);
		if (Hero)
		{   
			// Set Walking State
			float HeroSpeed = Hero->GetMovementComponent()->Velocity.Size();
			if (HeroSpeed > 5.0f&&bIsAiming == false)
				bIsWalking = true;
			else
				bIsWalking = false;

			// Set Jumping State
			bIsJumping = Hero->GetMovementComponent()->IsFalling();

		}
	}
}