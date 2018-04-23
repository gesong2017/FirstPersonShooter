// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ProjectMacroLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/HeroGun.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Hero CapsuleComponent Collision, has nothing to do with damage check
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PICKUP,ECR_Block);

	// Intialize the actual mesh that will be seen or acted with other actors in game
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Ignore);

	// Initialize First Person Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->bOnlyOwnerSee = true;
	FirstPersonMesh->bOwnerNoSee = false;
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	FirstPersonMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FirstPersonMesh->SetCollisionObjectType(ECC_Pawn);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	// Spawn a Hero Gun and attach to hero
	if (HeroGun_BP)
	{
		FActorSpawnParameters GunSpawnParameter;
		GunSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AHeroGun* CurrentGun = GetWorld()->SpawnActor<AHeroGun>(HeroGun_BP, GunSpawnParameter);
		if (CurrentGun)
		{
			CurrentGun->SetActorHiddenInGame(false);
			CurrentGun->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPoint"));
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("HeroGun_BP is not valid"));
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);

	// Bind rotation events
	PlayerInputComponent->BindAxis("LookUp", this, &AHero::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AHero::AddControllerYawInput);

}

void AHero::MoveForward(float Value)
{
	if (Controller != nullptr&&Value != 0.0f)
	{   
		// If player is in the air, use actor rotation instead of controller rotation
		bool bLimitHeroRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		FRotator Rotation = bLimitHeroRotation ? GetActorRotation() : Controller->GetControlRotation();
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHero::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		//// If player is in the air, use actor rotation instead of controller rotation
		//bool bLimitHeroRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		//FRotator Rotation = bLimitHeroRotation ? GetActorRotation() : Controller->GetControlRotation();
		FRotator Rotation = GetActorRotation();
		FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


