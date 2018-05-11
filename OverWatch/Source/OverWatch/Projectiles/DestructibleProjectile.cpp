// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleProjectile.h"
#include "DestructibleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hero/Hero.h"
#include "AI/AICharacters/LargeMonster.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADestructibleProjectile::ADestructibleProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and Initialize DestructibleMesh Comp
	DestructibleComp = CreateDefaultSubobject<UDestructibleComponent>(TEXT("DestructibleComp"));
	RootComponent = DestructibleComp;
	DestructibleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestructibleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	DestructibleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	DestructibleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	// Create and Initialize projectile movement comp
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->UpdatedComponent = DestructibleComp;
	ProjectileMovementComp->InitialSpeed = 2000.0f;
	ProjectileMovementComp->MaxSpeed = 2000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	// Initialize Variables
	bIsDestructible = false;
	bCanDamageHero = false;
	HitTimes = 0;
}

// Called when the game starts or when spawned
void ADestructibleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Events to our own function
	DestructibleComp->OnComponentHit.AddDynamic(this, &ADestructibleProjectile::OnHitHero);
	DestructibleComp->OnComponentFracture.AddDynamic(this, &ADestructibleProjectile::OnMeshDestructed);
	ProjectileMovementComp->OnProjectileStop.AddDynamic(this, &ADestructibleProjectile::OnFinishFlying);

	// Initialize Variables
	ProjectileMovementComp->SetActive(false);
	bIsDestructible = false;
	bCanDamageHero = false;
	HitTimes = 0;
}

void ADestructibleProjectile::OnHitHero(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{   
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero && bCanDamageHero == true)
	{   
		HitTimes++;
		if (HitTimes == 1)
		{
			UGameplayStatics::ApplyDamage(Hero, ProjectileDamage, GetInstigatorController(), this, ProjectileDamageType);
			UE_LOG(LogTemp, Warning, TEXT("You Have Been Hit By Projectile"))
		}
	}

}

void ADestructibleProjectile::OnMeshDestructed(const FVector & HitPoint, const FVector & HitDirection)
{
	// Disable Collision Check and set auto destory
	UE_LOG(LogTemp, Warning, TEXT("Projectile Has Been Destroyed"))
	ALargeMonster* LargeMonster = Cast<ALargeMonster>(GetOwner());
	if (LargeMonster)
	{
		int IndexOfThisProjectile = LargeMonster->GetIndexOfStone(this);
		if (IndexOfThisProjectile >= 0)
		{
			LargeMonster->GetAllStones().RemoveAtSwap(IndexOfThisProjectile, 1, true);
			UE_LOG(LogTemp, Warning, TEXT("Successfully removed this destructed projectile from the list"))
			UE_LOG(LogTemp, Warning, TEXT("Current number of pickup available is %d"), LargeMonster->GetAllStones().Num())
		}
	}
	
	DestructibleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	bCanDamageHero = false;
	HitTimes = 0;
	SetLifeSpan(10.0f);
}

void ADestructibleProjectile::OnFinishFlying(const FHitResult & ImpactResult)
{
	// Diable Damage To Hero and Set
	UE_LOG(LogTemp, Warning, TEXT("Projectile Has finished flying"))

	// Reset Variables
	bIsDestructible = false;
	bCanDamageHero = false;
	HitTimes = 0;
}

void ADestructibleProjectile::IntializeProjectile(AActor * TargetActor)
{   
	// Detach from monster's hand
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Activate Projectile Movement Comp and Initialize it
	ProjectileMovementComp->SetActive(true);
	FVector ShotDirection = TargetActor->GetActorLocation() - GetActorLocation();
	ProjectileMovementComp->Velocity = ShotDirection.GetSafeNormal() * ProjectileMovementComp->InitialSpeed;
	bIsDestructible = true;
	bCanDamageHero = true;
}


