// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeComponent.h"
#include "Hero/Hero.h"
#include "Kismet/GameplayStatics.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


// Sets default values
AMeleeZombie::AMeleeZombie()
{   
	// Create Zombie Box Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->bEditableWhenInherited = true;

	// Initialize Variable
	HitTimes = 0;
	bIsTargetSet = false;
	MaxiumSpeed = 200.0f;
	NormalSpeed = 200.0f;
	TimesHitByHero = 0;
}

// Called when the game starts or when spawned
void AMeleeZombie::BeginPlay()
{
	Super::BeginPlay();

	// Register collision event
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeZombie::OnHit);
		CollisionBox->bGenerateOverlapEvents = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("CollisionBox attach to : %s"), *CollisionBox->GetAttachSocketName().ToString())

	// Initialize max walk speed to normal speed
	UCharacterMovementComponent* ZombieMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (ZombieMovementComponent)
		ZombieMovementComponent->MaxWalkSpeed = NormalSpeed;
}

void AMeleeZombie::OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser)
{
	TimesHitByHero++;

	if (Health <= 0.0f && bIsAlive == true)
		GetKilled();

	else
	{   
		if (bIsTargetSet == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Zombie Get Hit, Find DamageCauser"))
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

		// Update Zombie Speed if this is the first time get hurt by hero
		if (TimesHitByHero == 1)
		{
			// update max walk speed to maxium speed
			UCharacterMovementComponent* ZombieMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (ZombieMovementComponent)
				ZombieMovementComponent->MaxWalkSpeed = MaxiumSpeed;
		}
	}

}

void AMeleeZombie::OnTargetSensed(APawn * Pawn)
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

void AMeleeZombie::OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{   
	if (HitTimes == 0)
	{
		AHero* Hero = Cast<AHero>(OtherActor);
		if (Hero)
		{
			float BaseDamage = GetAIAttributes()->GetBaseDamage();
			UGameplayStatics::ApplyDamage(Hero, BaseDamage, GetInstigatorController(), this, ZombieDamageType);
			HitTimes++;
			UE_LOG(LogTemp, Warning, TEXT("Hero Hit By Zombie"))
		}
	}
}



