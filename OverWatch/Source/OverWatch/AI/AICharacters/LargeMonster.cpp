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
#include "Projectiles/DestructibleProjectile.h"
#include "DestructibleComponent.h"

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

	// Set the stones owner
	if (Stones.Num() > 0)
	{
		for (int i = 0; i < Stones.Num(); i++)
		{
			Stones[i]->SetOwner(this);
		}
	}
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

AActor* ALargeMonster::GetClosestStoneByManhattanDistance()
{   
	if (Stones.Num() > 0)
	{
		AActor* ClosestStone = Stones[0];
		for (int i = 1; i < Stones.Num(); i++)
		{
			// Current Closest Stone Manhattan Distance To Monster
			FVector ClosestDistanceVector = ClosestStone->GetActorLocation() - GetActorLocation();
			float ClosestManhattanDistance = FMath::Abs(ClosestDistanceVector.X + ClosestDistanceVector.Y);

			// Current Index Stone Manhattan Distance To Monster
			FVector CurrentDistanceVector = Stones[i]->GetActorLocation() - GetActorLocation();
			float CurrentManhattanDistance = FMath::Abs(CurrentDistanceVector.X + CurrentDistanceVector.Y);

			if (ClosestManhattanDistance > CurrentManhattanDistance)
				ClosestStone = Stones[i];
		}
		return ClosestStone;
	}
	return nullptr;
}

int ALargeMonster::GetIndexOfStone(AActor* TargetActor) 
{
	for (int i = 0; i < Stones.Num(); i++)
	{
		if (Stones[i]->GetName() == TargetActor->GetName())
		{   
			UE_LOG(LogTemp, Warning, TEXT("Found Destructed Stone In the Array!"))
			return i;
		}
	}
	return -1;
}

void ALargeMonster::Pickup()
{
	ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());
	if (BaseAIController)
	{
		UBlackboardComponent* BlackboardComp = BaseAIController->GetBlackboardComp();
		if (BlackboardComp)
		{
			AActor* ClosestStone = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BaseAIController->GetKeyID_ClosestPickup()));
			if (ClosestStone)
			{
				ADestructibleProjectile* StoneProjectile = Cast<ADestructibleProjectile>(ClosestStone);
				if (StoneProjectile)
				{   
					UE_LOG(LogTemp, Warning, TEXT("Cast to stone projectile success!"))
					//StoneProjectile->GetDestructibleComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					StoneProjectile->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandPickupSocket"));
				}
			}
		}
	}
}

void ALargeMonster::Throw()
{
	ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());
	if (BaseAIController)
	{
		UBlackboardComponent* BlackboardComp = BaseAIController->GetBlackboardComp();
		if (BlackboardComp)
		{
			AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BaseAIController->GetKeyID_TargetActor()));
			if (TargetActor)
			{
				AActor* ClosestStone = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BaseAIController->GetKeyID_ClosestPickup()));
				if (ClosestStone)
				{
					ADestructibleProjectile* StoneProjectile = Cast<ADestructibleProjectile>(ClosestStone);
					if (StoneProjectile)
					{
						UE_LOG(LogTemp, Warning, TEXT("Monster is throwing stone projectile !"))
						StoneProjectile->IntializeProjectile(TargetActor);

					}
				}
			}
		}
	}
}

