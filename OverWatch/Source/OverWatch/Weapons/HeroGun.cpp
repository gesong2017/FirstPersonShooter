// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroGun.h"
#include "Components/SkeletalMeshComponent.h"

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
}




