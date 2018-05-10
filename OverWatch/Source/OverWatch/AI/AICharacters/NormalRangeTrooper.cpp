// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalRangeTrooper.h"
#include "Components/SkeletalMeshComponent.h"
#include "Hero/Hero.h"
#include "Weapons/AIGun.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ConstructorHelpers.h"


// Sets default values
ANormalRangeTrooper::ANormalRangeTrooper()
{   
	// Find hero gun class
	static ConstructorHelpers::FClassFinder<AAIGun> AIGunBlueprint(TEXT("/Game/Blueprints/Weapons/AIGun_BP"));
	AIGun_BP = AIGunBlueprint.Class;

	// Initialize Variable
	bIsTargetSet = false;
	AIGun = nullptr;
}

void ANormalRangeTrooper::BeginPlay()
{
	Super::BeginPlay();

	// Spawn an AI Gun and attach to AI Character
	if (AIGun_BP)
	{
		FActorSpawnParameters GunSpawnParameter;
		GunSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AIGun = GetWorld()->SpawnActor<AAIGun>(AIGun_BP, GunSpawnParameter);
		if (AIGun)
		{
			AIGun->SetOwner(this);
			AIGun->SetActorHiddenInGame(false);
			// Attach third person weapon mesh to third person hero mesh, Attach first person weapon mesh to first person hero mesh, 
			AIGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RifleSocket"));
		}
	}
}

void ANormalRangeTrooper::OnHealthChanged(UAttributeComponent * AttributeComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * IntigatedBy, AActor * DamageCauser)
{   
	UE_LOG(LogTemp, Warning, TEXT("Normal Range Trooper Get Hit"))

	if (Health <= 0.0f && bIsAlive == true)
	{   
		GetKilled();
		AIGun->SetLifeSpan(10.0f);
	}
	else
	{
		if (bIsTargetSet == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Normal Range Trooper, Find DamageCauser"))
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

void ANormalRangeTrooper::OnTargetSensed(APawn * Pawn)
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

void ANormalRangeTrooper::Fire(AActor* TargetActor)
{
	if (TargetActor)
		AIGun->Fire(TargetActor);
}
