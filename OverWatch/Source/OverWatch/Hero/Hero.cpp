// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "AttributeComponent.h"
#include "ProjectMacroLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/HeroGun.h"
#include "ConstructorHelpers.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "HeroFPPAnimInstance.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	// Create a actor attribute component
	HeroAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("HeroAttributes"));

	// Find hero gun class
	static ConstructorHelpers::FClassFinder<AHeroGun> HeroGunBlueprint(TEXT("/Game/Blueprints/Weapons/HeroGun_BP"));
	HeroGun_BP = HeroGunBlueprint.Class;

	// Initialize variables here
	HeroGun = nullptr;
	NumOfBulletsLeftOnHero = 72;
	bIsAlive = true;
	LastFireTime = 0.0f;
	bCanFire = true;
	bIsReloading = false;
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
		HeroGun = GetWorld()->SpawnActor<AHeroGun>(HeroGun_BP, GunSpawnParameter);
		if (HeroGun)
		{   
			HeroGun->SetOwner(this);
			HeroGun->SetActorHiddenInGame(false);
			// Attach third person weapon mesh to third person hero mesh, Attach first person weapon mesh to first person hero mesh, 
			HeroGun->GetWeaponSkeletalMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPoint"));
			HeroGun->GetFirstPersonWeaponSkeletalMesh()->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPoint"));
		}
	}
		//UE_LOG(LogTemp, Warning, TEXT("HeroGun_BP is not valid"));
	HeroAttributes->OnHealthChanged.AddDynamic(this, &AHero::OnHealthChanged);
}


// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHero::MoveRight);

	// Bind rotation events
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHero::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHero::AddControllerYawInput);

	// Bind Fire and Reload Events
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AHero::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AHero::StopFire);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AHero::Reload);
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

void AHero::StartFire()
{
	if (HeroGun)
	{   
		float TimeBetweenShots = HeroGun->GetDeltaTimeBetweenShots();
		float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(AutomaticFireTimer, this, &AHero::FireOnce, TimeBetweenShots, true, FirstDelay);
	}
}

void AHero::StopFire()
{
	if (HeroGun)
	{   
		int BulletsLeftOnGun = HeroGun->GetCurrentNumberOfBullets();
		GetWorldTimerManager().ClearTimer(AutomaticFireTimer);
		if (BulletsLeftOnGun == 0)
			Reload();
	}
}

void AHero::FireOnce()
{   
	if (HeroGun && bCanFire == true)
	{
		int BulletsLeftOnGun = HeroGun->GetCurrentNumberOfBullets();

		if (BulletsLeftOnGun > 0)
			HeroGun->Fire();

		else
			Reload();
	}
}

void AHero::Reload()
{
	if (HeroGun && bIsReloading == false)
	{   
		// Check how many bullets left on gun
		int MaxiumBulletsOnGun = HeroGun->GetMaxiumNumberOfBullets();
		int BulletsLeftOnGun = HeroGun->GetCurrentNumberOfBullets();

		// if current number of bullets on gun is less than maxium load and hero still has additional bullets, then we play reload animation montage
		if (BulletsLeftOnGun < MaxiumBulletsOnGun && NumOfBulletsLeftOnHero > 0)
			StartReload();

		// else, do something to tell player that no bullets left
		else
		{

		}
	}
}

void AHero::StartReload()
{   
	bCanFire = false;
	bIsReloading = true;
	float AnimationDuration = PlayReloadAnimMontage();
	if (AnimationDuration <= 0.0f)
		AnimationDuration = 0.5f;

	// After Animation is over, stop reload and update bullets on gun
	GetWorldTimerManager().SetTimer(StopReloadTimer, this, &AHero::StopReload, AnimationDuration, false);
	GetWorldTimerManager().SetTimer(ReloadGunTimer, this, &AHero::ReloadGun, FMath::Max(0.1f, AnimationDuration - 0.1f), false);
}

void AHero::StopReload()
{   
	bCanFire = true;
	bIsReloading = false;
	StopReloadAnimMontage();
}

void AHero::ReloadGun()
{   
	// Check how many bullets left on gun
	int MaxiumBulletsOnGun = HeroGun->GetMaxiumNumberOfBullets();
	int BulletsLeftOnGun = HeroGun->GetCurrentNumberOfBullets();

	//check if we can do a full reload
	int totalNumberOfBullets = BulletsLeftOnGun + NumOfBulletsLeftOnHero;
	if (totalNumberOfBullets > MaxiumBulletsOnGun)
	{
		HeroGun->UpdateCurrentNumberOfBullets(MaxiumBulletsOnGun);
		int NumOfReloadedBullets = MaxiumBulletsOnGun - BulletsLeftOnGun;
		NumOfBulletsLeftOnHero = NumOfBulletsLeftOnHero - NumOfReloadedBullets;
	}
	else
	{
		HeroGun->UpdateCurrentNumberOfBullets(totalNumberOfBullets);
		NumOfBulletsLeftOnHero = 0;
	}
}

void AHero::OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && bIsAlive == true)
	{
		// Die
		bIsAlive = false;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->bOnlyOwnerSee = true;
		GetMesh()->bOwnerNoSee = false;
		FirstPersonMesh->bOnlyOwnerSee = false;
		FirstPersonMesh->bOwnerNoSee = true;
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);

		// Detach and destroy gun
		HeroGun->GetWeaponSkeletalMesh()->bOnlyOwnerSee = true;
		HeroGun->GetWeaponSkeletalMesh()->bOwnerNoSee = false;
		HeroGun->GetFirstPersonWeaponSkeletalMesh()->bOnlyOwnerSee = false;
		HeroGun->GetFirstPersonWeaponSkeletalMesh()->bOwnerNoSee = true;
		HeroGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		HeroGun->SetLifeSpan(10.0f);
	}
}

FRotator AHero::GetAimOffsets() const
{
	FVector AimDirectionWS = GetBaseAimRotation().Vector();
	FVector AimDirectionLS = ActorToWorld().InverseTransformVectorNoScale(AimDirectionWS);
	return AimDirectionLS.Rotation();
}

void AHero::GetHeroCameraInformation(FVector & outPosition, FRotator & outRotation)
{
	outPosition = FirstPersonCamera->GetComponentLocation();
	outRotation = FirstPersonCamera->GetComponentRotation();
}

float AHero::PlayReloadAnimMontage(float InPlayRate, FName StartSectionName)
{
	UHeroFPPAnimInstance* HeroFirstPersonAnimInstance = Cast<UHeroFPPAnimInstance>(FirstPersonMesh->GetAnimInstance());
	if (HeroFirstPersonAnimInstance)
	{
		UAnimMontage* ReloadMontage = HeroFirstPersonAnimInstance->GetReloadAnimMontage();
		if (ReloadMontage)
			return HeroFirstPersonAnimInstance->Montage_Play(ReloadMontage, InPlayRate);
	}
	return 0.0f;
}

void AHero::StopReloadAnimMontage()
{
	UHeroFPPAnimInstance* HeroFirstPersonAnimInstance = Cast<UHeroFPPAnimInstance>(FirstPersonMesh->GetAnimInstance());
	if (HeroFirstPersonAnimInstance)
	{
		UAnimMontage* ReloadMontage = HeroFirstPersonAnimInstance->GetReloadAnimMontage();
		if (ReloadMontage)
			HeroFirstPersonAnimInstance->Montage_Stop(ReloadMontage->BlendOut.GetBlendTime(), ReloadMontage);
	}
}


