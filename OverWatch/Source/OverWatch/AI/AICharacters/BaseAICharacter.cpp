// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAICharacter.h"
#include "BaseAIController.h"
#include "ProjectMacroLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Link AI Controller
	AIControllerClass = ABaseAIController::StaticClass();

	// Set NPC CapsuleComponent Collision, has nothing to do with damage check
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Block);

	// Intialize the skeletal mesh
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PICKUP, ECR_Ignore);

	// Create an actor attribute component
	AIAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("AIAttributes"));

	// Create and Initialize Pawn Sensing Component
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	// Initialize Variables
	bIsAlive = true;
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// bind delegates to our own function
	AIAttributes->OnHealthChanged.AddDynamic(this, &ABaseAICharacter::OnHealthChanged);
	PawnSensing->OnSeePawn.AddDynamic(this, &ABaseAICharacter::OnTargetSensed);
}

void ABaseAICharacter::GetKilled()
{
	// Kill the bot
	bIsAlive = false;
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.0f);
}

void ABaseAICharacter::OnHealthChanged(UAttributeComponent * AttributeComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * IntigatedBy, AActor * DamageCauser)
{
}

void ABaseAICharacter::OnTargetSensed(APawn * Pawn)
{
}

