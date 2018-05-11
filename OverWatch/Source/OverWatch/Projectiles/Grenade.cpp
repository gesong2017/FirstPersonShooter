// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AGrenade::AGrenade()
{
	// Use a static mesh to represent grenade
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	// Initialize Variable
	GrenadeDamage = 0.0f;
	InitialLifeSpan = 15.0f;
	ExplosionDelay = 0.0f;
	HitTimes = 0;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{   
	HitTimes++;
	if (HitTimes == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grenade Hit Something"))
		Explode(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	}
}

void AGrenade::InitializeVelocity(FVector BulletVelocity)
{
	ProjectileMovementComp->Velocity = BulletVelocity;
}

