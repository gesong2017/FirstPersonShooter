// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Projectiles/LaserBullet.h"
#include "Hero/Hero.h"
#include "Kismet/GameplayStatics.h"
#include "EffectsLibrary.h"

// Sets default values
AAIGun::AAIGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize gun mesh
	WeaponSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponSkeletalMesh->CastShadow = false;
	WeaponSkeletalMesh->bCastDynamicShadow = false;
	WeaponSkeletalMesh->bReceivesDecals = false;
	WeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Find laser bullet class
	static ConstructorHelpers::FClassFinder<ALaserBullet> LaserBulletBlueprint(TEXT("/Game/Blueprints/Projectiles/LaserBullet_BP"));
	LaserBullet_BP = LaserBulletBlueprint.Class;

	// Initialize variable
	MuzzleSocketName = "MuzzleSocket";
	BulletSpeed = 3000.0f;
}

void AAIGun::BeginPlay()
{
	Super::BeginPlay();

	DivByBulletSpeed = 1 / BulletSpeed;
}

void AAIGun::Fire(AActor* Target)
{
	// try and fire a bullet
	if (LaserBullet_BP)
	{   
		APawn* GunOwner = Cast<APawn>(GetOwner());
		if (GunOwner)
		{   
			// Check if the ai's target is the player
			AHero* TargetEnemy = Cast<AHero>(Target);
			if (TargetEnemy)
			{
				// Correctly Spawn the bullet
				const FRotator SpawnRotation = WeaponSkeletalMesh->GetSocketRotation(MuzzleSocketName);
				const FVector SpawnLocation = WeaponSkeletalMesh->GetSocketLocation(MuzzleSocketName);
				
				// AI fire predication
				FVector TraceEnd = TargetEnemy->GetActorLocation();
				FVector FlyDir;
				float FlyDistance;
				(TraceEnd - SpawnLocation).ToDirectionAndLength(FlyDir, FlyDistance);
				float FlyTime = FlyDistance * DivByBulletSpeed;
				FVector PredictedPosition = TraceEnd + (TargetEnemy->GetVelocity() * FlyTime);

				// Finalize Bullet fly direction
				FlyDir = (PredictedPosition - SpawnLocation).GetSafeNormal();

				// Set Spawn Transform
				FTransform spawnTransform;
				spawnTransform.SetLocation(SpawnLocation);
				spawnTransform.SetRotation(SpawnRotation.Quaternion());
				spawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

				// spawn the projectile at the muzzle
				ALaserBullet* LaserBullet = GetWorld()->SpawnActorDeferred<ALaserBullet>(LaserBullet_BP, spawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				if (LaserBullet)
				{   
					LaserBullet->FinishSpawning(spawnTransform);
					FVector BulletVelocity = FlyDir * BulletSpeed;
					LaserBullet->InitializeVelocity(BulletVelocity);
				}

				// Play Particle Effect at muzzle location
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UEffectsLibrary::GetVFX(EVFXType::MuzzleFire), SpawnLocation);
			}
		}
	}
}

