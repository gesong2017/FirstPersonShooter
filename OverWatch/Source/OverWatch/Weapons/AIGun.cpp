// Fill out your copyright notice in the Description page of Project Settings.

#include "AIGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Projectiles/LaserBullet.h"
#include "Projectiles/Grenade.h"
#include "Hero/Hero.h"
#include "Kismet/GameplayStatics.h"
#include "EffectsLibrary.h"
#include "TimerManager.h"

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

	// Find grenade classes
	static ConstructorHelpers::FClassFinder<AGrenade> NormalGrenadeBlueprint(TEXT("/Game/Blueprints/Projectiles/NormalGrenade_BP"));
	NormalGrenade_BP = NormalGrenadeBlueprint.Class;

	static ConstructorHelpers::FClassFinder<AGrenade> SmokeGrenadeBlueprint(TEXT("/Game/Blueprints/Projectiles/SmokeGrenade_BP"));
	SmokeGrenade_BP = SmokeGrenadeBlueprint.Class;

	// Initialize variable
	MuzzleSocketName = "MuzzleSocket";
	BulletSpeed = 3000.0f;
	bCanFireSmokeGrenade = true;
}

void AAIGun::BeginPlay()
{
	Super::BeginPlay();

	DivByBulletSpeed = 1 / BulletSpeed;
}

void AAIGun::Fire(AActor* Target)
{   
	if (GunType == EAIGunType::Rifle)
		RifleFire(Target);

	else if (GunType == EAIGunType::GrenadeLanucher)
		GrenadeFire(Target);
}

void AAIGun::RifleFire(AActor * Target)
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

				// Add Random Offset for AI
				float VerticalOffset = FMath::RandRange(-50.0f, 100.0f);
				float HorizontalOffset_X = FMath::RandRange(-100.0f, 100.0f);
				float HorizontalOffset_Y = FMath::RandRange(-100.0f, 100.0f);

				// Override Predicted Location with offset value
				PredictedPosition += FVector(HorizontalOffset_X, HorizontalOffset_Y, VerticalOffset);

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

void AAIGun::GrenadeFire(AActor * Target)
{
	if (NormalGrenade_BP && SmokeGrenade_BP)
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

				// Add Random Offset for AI
				float VerticalOffset = FMath::RandRange(-50.0f, 150.0f);
				float HorizontalOffset_X = FMath::RandRange(-100.0f, 100.0f);
				float HorizontalOffset_Y = FMath::RandRange(-100.0f, 100.0f);

				// Override Predicted Location with offset value
				PredictedPosition += FVector(HorizontalOffset_X, HorizontalOffset_Y, VerticalOffset);


				// Finalize Bullet fly direction
				FlyDir = (PredictedPosition - SpawnLocation).GetSafeNormal();

				// Set Spawn Transform
				FTransform spawnTransform;
				spawnTransform.SetLocation(SpawnLocation);
				spawnTransform.SetRotation(SpawnRotation.Quaternion());
				spawnTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

				// spawn the correct projectile at the muzzle
				if (bCanFireSmokeGrenade == true)
				{
					AGrenade* SmokeGrenade = GetWorld()->SpawnActorDeferred<AGrenade>(SmokeGrenade_BP, spawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (SmokeGrenade)
					{   
						bCanFireSmokeGrenade = false;
						SmokeGrenade->FinishSpawning(spawnTransform);
						GetWorldTimerManager().ClearTimer(SmokeGrenadeCoolDown);
						GetWorldTimerManager().SetTimer(SmokeGrenadeCoolDown, this, &AAIGun::EnableSmokeGrenade, 30.0f, false);
						FVector BulletVelocity = FlyDir * BulletSpeed;
						SmokeGrenade->InitializeVelocity(BulletVelocity);
					}
				}

				else
				{
					AGrenade* NormalGrenade = GetWorld()->SpawnActorDeferred<AGrenade>(NormalGrenade_BP, spawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
					if (NormalGrenade)
					{
						NormalGrenade->FinishSpawning(spawnTransform);
						FVector BulletVelocity = FlyDir * BulletSpeed;
						NormalGrenade->InitializeVelocity(BulletVelocity);
					}
				}	
			}
		}
	}
}

