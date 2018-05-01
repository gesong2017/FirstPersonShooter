// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AttributeComponent.h"
#include "Hero/Hero.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ProjectMacroLibrary.h"


// Sets default values
AMeleeZombie::AMeleeZombie()
{   
	// Create Zombie Box Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->bEditableWhenInherited = true;

	// Initialize Variable
	HitTimes = 0;
}

// Called when the game starts or when spawned
void AMeleeZombie::BeginPlay()
{
	Super::BeginPlay();

	// Register collision event
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeZombie::OnCollisionBoxOverlapBegin);
		CollisionBox->bGenerateOverlapEvents = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("CollisionBox attach to : %s"), *CollisionBox->GetAttachSocketName().ToString())

}

void AMeleeZombie::OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && bIsAlive == true)
		GetKilled();
	else
		UE_LOG(LogTemp, Warning, TEXT("Zombie Health Changed"))
}

void AMeleeZombie::OnCollisionBoxOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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
