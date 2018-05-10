// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Hero/Hero.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectMacroLibrary.h"
#include "EffectsLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Hero/HeroController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "AttributeComponent.h"
#include "UI/InGameHUD.h"
#include "Projectiles/DestructibleProjectile.h"


// Sets default values
AHeroGun::AHeroGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize tpp weapon mesh component
	WeaponSkeletalMesh->bOnlyOwnerSee = false;
	WeaponSkeletalMesh->bOwnerNoSee = true;
	WeaponSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponSkeletalMesh->CastShadow = false;
	WeaponSkeletalMesh->bCastDynamicShadow = false;
	WeaponSkeletalMesh->bReceivesDecals = false;
	WeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Initialize a fpp mesh component for hero gun
	FPPWeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonWeaponSkeletalMesh"));
	FPPWeaponSkeletalMesh->SetupAttachment(WeaponSkeletalMesh);
	FPPWeaponSkeletalMesh->bOnlyOwnerSee = true;
	FPPWeaponSkeletalMesh->bOwnerNoSee = false;
	FPPWeaponSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	FPPWeaponSkeletalMesh->CastShadow = false;
	FPPWeaponSkeletalMesh->bCastDynamicShadow = false;
	FPPWeaponSkeletalMesh->bReceivesDecals = false;
	FPPWeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	FPPWeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPPWeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Initialize Weapon Bullets
	MaxiumNumberOfBullets = 36;
	CurrentNumberOfBullets = 36;
	MuzzleSocketName = "MuzzleFlashSocket";
	GunFireRate = 600.0f;
	TimeBetweenShots = 0.0f;
}

void AHeroGun::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60.0f / GunFireRate;
}

void AHeroGun::Fire()
{   
	AHero* Hero = Cast<AHero>(GetOwner());
	if (Hero)
	{   
		CurrentNumberOfBullets--;
		//Play VFX at the muzzle location
		UParticleSystem* MuzzleFire = UEffectsLibrary::GetVFX(EVFXType::MuzzleFire);
		UGameplayStatics::SpawnEmitterAttached(MuzzleFire, FPPWeaponSkeletalMesh, MuzzleSocketName);

		// Calculate Line Trace Start and End point
		FVector CameraLocation;
		FRotator CameraRotation;
		Hero->GetHeroCameraInformation(CameraLocation, CameraRotation);
		FVector ShotDirection = CameraRotation.Vector();
		FVector TraceEnd = CameraLocation + (ShotDirection * 10000.0f);

		// Initialize CollisionQueryParams
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.bReturnPhysicalMaterial = true;

		// Use Line Trace to simulate our bullet
		FHitResult HitResult;
		bool HitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, COLLISION_WEAPON, CollisionQueryParams);
		
		// Only do stuff when our bullet hit something
		if (HitSomething == true)
		{   
			// Set the bullet end point to impactpoint
			TraceEnd = HitResult.ImpactPoint;

			// Get the surface type of the things we hit
			EPhysicalSurface HitPointSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			// Play different effects based on surface type
			PlayImpactEffectsBasedOnSurfaceType(HitPointSurfaceType, TraceEnd);

			// check if the bullet hit an ai character or other stuff
			ABaseAICharacter* AIGotHit = Cast<ABaseAICharacter>(HitResult.GetActor());
			ADestructibleProjectile* ObjectGotHit = Cast<ADestructibleProjectile>(HitResult.GetActor());
			if (AIGotHit)
			{   
				float GunDamage = Hero->GetHeroAttribute()->GetBaseDamage();
				// Head Shot
				if (HitPointSurfaceType == SURFACE_FLESHVULNERABLE)
				{
					GunDamage = 4.0f * GunDamage;
					UE_LOG(LogTemp, Warning, TEXT("Head Shot"))
				}

				UGameplayStatics::ApplyPointDamage(AIGotHit, GunDamage, ShotDirection, HitResult, Hero->GetInstigatorController(), this, DamageType);
			}

			if (ObjectGotHit)
			{
				if (ObjectGotHit->GetIsDestructible() == true)
					UGameplayStatics::ApplyPointDamage(ObjectGotHit, 1.0f, ShotDirection, HitResult, Hero->GetInstigatorController(), this, DamageType);
				else
					UE_LOG(LogTemp, Warning, TEXT("Hitted Projectile, But it is not destructible yet"))
			}
		}

		// Spawn a bullet trail anyway if we didn't hit anything
		UParticleSystem* BulletTrail = UEffectsLibrary::GetVFX(EVFXType::BulletTrail);
		FVector MuzzleLocation = FPPWeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TrailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletTrail, MuzzleLocation);
		if (TrailComp)
			TrailComp->SetVectorParameter(TEXT("BeamEnd"), TraceEnd);

		// Simulate Fire Recoil use camera shake and Update UI
		AHeroController* HeroController = Cast<AHeroController>(Hero->GetController());
		if (HeroController&&FireRecoil)
		{
			HeroController->ClientPlayCameraShake(FireRecoil);
			HeroController->GetInGameHUD()->UpdateNumOfBulletsOnGun(CurrentNumberOfBullets);
		}

		// Update the last time we fired
		Hero->SetLastFireTime(GetWorld()->TimeSeconds);


	}

}

void AHeroGun::PlayImpactEffectsBasedOnSurfaceType(EPhysicalSurface surfaceType, FVector Location)
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




