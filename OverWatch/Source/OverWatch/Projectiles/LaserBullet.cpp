// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserBullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Hero/Hero.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ProjectMacroLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EffectsLibrary.h"

ALaserBullet::ALaserBullet()
{   
	// Use a particle to represent laser
	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	ParticleSystemComp->SetupAttachment(RootComponent);

	// Initialize Variable
	BulletBaseDamage = 0.0f;

	InitialLifeSpan = 10.0f;
}

void ALaserBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBullet::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{   
	// Get the surface type of the things we hit and play effects
	EPhysicalSurface HitPointSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	FVector ImpactLocation = Hit.ImpactPoint;
	PlayImpactEffectsBasedOnSurfaceType(HitPointSurfaceType, ImpactLocation);

	// Check if bullet hits the player
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{   
		UE_LOG(LogTemp, Warning, TEXT("You are hit by laser bullets %s"), *Hit.BoneName.ToString())

		// Head Shot!
		if (HitPointSurfaceType == SURFACE_FLESHVULNERABLE)
			BulletBaseDamage = BulletBaseDamage * 4.0f;

		// Process Damage
	    UGameplayStatics::ApplyDamage(Hero, BulletBaseDamage, GetInstigatorController(), this, BulletDamageType);
	}

	// Destroy itself
	Destroy();
}

void ALaserBullet::PlayImpactEffectsBasedOnSurfaceType(EPhysicalSurface surfaceType, FVector Location)
{
	UParticleSystem* vfx = nullptr;
	switch (surfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		vfx = UEffectsLibrary::GetVFX(EVFXType::BloodImpact);
		break;

	default:
		vfx = UEffectsLibrary::GetVFX(EVFXType::GenericImpact);
		break;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), vfx, Location);
}

void ALaserBullet::InitializeVelocity(FVector BulletVelocity)
{
	ProjectileMovementComp->Velocity = BulletVelocity;
}
