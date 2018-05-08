// Fill out your copyright notice in the Description page of Project Settings.

#include "LargeMonster.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Hero/Hero.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"

// Sets default values
ALargeMonster::ALargeMonster()
{
	// Create Monster Collision Box
	LeftHand = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollisionBox"));
	LeftHand->bEditableWhenInherited = true;

	RightHand = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollisionBox"));
	RightHand->bEditableWhenInherited = true;

	// Initialize Variable
	HitTimes = 0;
	bIsTargetSet = false;
}

// Called when the game starts or when spawned
void ALargeMonster::BeginPlay()
{   
	Super::BeginPlay();
	// Register collision event
	if (LeftHand && RightHand)
	{
		LeftHand->OnComponentBeginOverlap.AddDynamic(this, &ALargeMonster::OnHit);
		LeftHand->bGenerateOverlapEvents = false;

		RightHand->OnComponentBeginOverlap.AddDynamic(this, &ALargeMonster::OnHit);
		RightHand->bGenerateOverlapEvents = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("LeftHandCollisionBox attach to : %s"), *LeftHand->GetAttachSocketName().ToString())
	UE_LOG(LogTemp, Warning, TEXT("RightHandCollisionBox attach to : %s"), *RightHand->GetAttachSocketName().ToString())
}

void ALargeMonster::OnHealthChanged(UAttributeComponent * AttributeComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * IntigatedBy, AActor * DamageCauser)
{   
	UE_LOG(LogTemp, Warning, TEXT("Monster Get Hit"))

	if (Health <= 0.0f && bIsAlive == true)
		GetKilled();
	else
	{
		if (bIsTargetSet == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster Get Hit, Find DamageCauser"))
				AHero* Hero = Cast<AHero>(DamageCauser->GetOwner());
			if (Hero)
			{
				ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());
				if (BaseAIController)
				{
					UBlackboardComponent* BlackboardComp = BaseAIController->GetBlackboardComp();
					if (BlackboardComp)
					{
						BlackboardComp->SetValue<UBlackboardKeyType_Object>(BaseAIController->GetKeyID_TargetActor(), Hero);
						bIsTargetSet = true;
					}
				}
			}
		}
	}
}

void ALargeMonster::OnTargetSensed(APawn * Pawn)
{
	if (bIsTargetSet == false)
	{
		AHero* Hero = Cast<AHero>(Pawn);
		if (Hero)
		{
			ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());
			if (BaseAIController)
			{
				UBlackboardComponent* BlackboardComp = BaseAIController->GetBlackboardComp();
				if (BlackboardComp)
				{
					BlackboardComp->SetValue<UBlackboardKeyType_Object>(BaseAIController->GetKeyID_TargetActor(), Hero);
					bIsTargetSet = true;
				}
			}
		}
	}
}

void ALargeMonster::OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (HitTimes == 0)
	{
		AHero* Hero = Cast<AHero>(OtherActor);
		if (Hero)
		{
			float BaseDamage = GetAIAttributes()->GetBaseDamage();
			UGameplayStatics::ApplyDamage(Hero, BaseDamage, GetInstigatorController(), this, MonsterDamageType);
			HitTimes++;
			UE_LOG(LogTemp, Warning, TEXT("Hero Hit By Zombie"))
		}
	}
}

