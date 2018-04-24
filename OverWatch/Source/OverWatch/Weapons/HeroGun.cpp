// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Hero/Hero.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHeroGun::AHeroGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize fpp weapon mesh component
	WeaponSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponSkeletalMesh->CastShadow = false;
	WeaponSkeletalMesh->bCastDynamicShadow = false;
	WeaponSkeletalMesh->bReceivesDecals = false;
	WeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//// Initialize a tpp mesh component for hero gun
	//FPPWeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonWeaponSkeletalMesh"));
	//FPPWeaponSkeletalMesh->SetupAttachment(WeaponSkeletalMesh);
	//FPPWeaponSkeletalMesh->bOnlyOwnerSee = true;
	//FPPWeaponSkeletalMesh->bOwnerNoSee = false;
	//FPPWeaponSkeletalMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	//FPPWeaponSkeletalMesh->CastShadow = false;
	//FPPWeaponSkeletalMesh->bCastDynamicShadow = false;
	//FPPWeaponSkeletalMesh->bReceivesDecals = false;
	//FPPWeaponSkeletalMesh->SetCollisionObjectType(ECC_WorldDynamic);
	//FPPWeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//FPPWeaponSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Initialize Weapon Bullets
	MaxiumNumberOfBullets = 36;
	CurrentNumberOfBullets = 36;
}

void AHeroGun::Fire()
{   
	AHero* Hero = Cast<AHero>(GetOwner());
	if (Hero)
	{   
		CurrentNumberOfBullets--;
		// Calculate Line Trace Start and End point
		FVector CameraLocation;
		FRotator CameraRotation;
		Hero->GetHeroCameraInformation(CameraLocation, CameraRotation);
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector()*10000.0f);

		// Initialize CollisionQueryParams
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceComplex = true;

		// Use Line Trace to simulate our bullet
		FHitResult HitResult;
		bool HitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, CollisionQueryParams);
		
		// Only do stuff when our bullet hit something
		if (HitSomething == true)
		{
			TraceEnd = HitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), TraceEnd, 10.0f, 5, FColor::Green, false, 5.0f, 0, 1.0f);
		}

		// show information for debug purpose
		DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 5.0f, 0, 1.0f);
	}

}




