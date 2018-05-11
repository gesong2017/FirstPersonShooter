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
	// Find ai gun class
	static ConstructorHelpers::FClassFinder<AAIGun> RifleBlueprint(TEXT("/Game/Blueprints/Weapons/AIRifle_BP"));
	Rifle_BP = RifleBlueprint.Class;

	static ConstructorHelpers::FClassFinder<AAIGun> GrenadeLauncherBlueprint(TEXT("/Game/Blueprints/Weapons/AIGrenadeLauncher_BP"));
	GrenadeLauncher_BP = GrenadeLauncherBlueprint.Class;

	// Initialize Variable
	bIsTargetSet = false;
	AIGun = nullptr;
}

void ANormalRangeTrooper::BeginPlay()
{
	Super::BeginPlay();

	// Spawn an AI Gun according to trooper type
	if (TrooperType == ERangeTrooperType::RifleTrooper)
	{
		if (Rifle_BP)
		{
			FActorSpawnParameters GunSpawnParameter;
			GunSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AIGun = GetWorld()->SpawnActor<AAIGun>(Rifle_BP, GunSpawnParameter);
			if (AIGun)
			{
				AIGun->SetOwner(this);
				AIGun->SetActorHiddenInGame(false);
				// Attach third person weapon mesh to third person hero mesh, Attach first person weapon mesh to first person hero mesh, 
				AIGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RifleSocket"));
			}
		}
	}

	else if (TrooperType == ERangeTrooperType::GrenadeTrooper)
	{
		if (GrenadeLauncher_BP)
		{
			FActorSpawnParameters GunSpawnParameter;
			GunSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AIGun = GetWorld()->SpawnActor<AAIGun>(GrenadeLauncher_BP, GunSpawnParameter);
			if (AIGun)
			{
				AIGun->SetOwner(this);
				AIGun->SetActorHiddenInGame(false);
				// Attach third person weapon mesh to third person hero mesh, Attach first person weapon mesh to first person hero mesh, 
				AIGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GrenadeLauncherSocket"));
			}
		}
	}

}

void ANormalRangeTrooper::OnHealthChanged(UAttributeComponent * AttributeComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * IntigatedBy, AActor * DamageCauser)
{   
	if (TrooperType == ERangeTrooperType::RifleTrooper)
	    UE_LOG(LogTemp, Warning, TEXT("Normal Range Trooper Get Hit"))

	else if (TrooperType == ERangeTrooperType::GrenadeTrooper)
		UE_LOG(LogTemp, Warning, TEXT("Grenade Trooper Get Hit"))

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
