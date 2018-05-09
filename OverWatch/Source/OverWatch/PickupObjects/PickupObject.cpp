// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Hero/Hero.h"
#include "Hero/HeroController.h"
#include "UI/InGameHUD.h"
#include "AttributeComponent.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and initialize static mesh of the pick up object
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create and initialize sphere comp
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Sphere Comp Overlap Event to our custom function
	if (SphereComp)
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::OnOverlapBegin);

	// Set This Actor Auto Destroy Able
	SetLifeSpan(30.0f);
}

void APickupObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{
		switch (ObjectType)
		{
		case EObjectType::Health:
			UpdateHeroHealth(Hero);
			break;
		case EObjectType::Ammo:
			UpdateHeroAmmo(Hero);
			break;
		default:
			break;
		}
	}
}

void APickupObject::UpdateHeroHealth(AHero* Hero)
{   
	// Only Add Player's Health When Player's health is not full
	float CurrentHealth = Hero->GetHeroAttribute()->GetCurrentHealth();
	float MaxiumHealth= Hero->GetHeroAttribute()->GetMaxiumHealth();
	if (CurrentHealth < MaxiumHealth)
	{   
		float HealthAfterAdded = CurrentHealth + 100.0f;
		HealthAfterAdded = FMath::Clamp(HealthAfterAdded, 100.0f, MaxiumHealth);

		// Update Hero's Health Value And UI
		Hero->GetHeroAttribute()->SetCurrentHealth(HealthAfterAdded);
		AHeroController* HeroController = Cast<AHeroController>(Hero->GetController());
		if (HeroController)
			HeroController->GetInGameHUD()->UpdateHealthValueAndProgressBar(HealthAfterAdded);

		// Destory this actor since it has been used
		Destroy();
	}
}

void APickupObject::UpdateHeroAmmo(AHero* Hero)
{   
	// Update Hero's Bullets Value And UI
	float CurrentBulletsLeftInBag = Hero->GetNumOfBulletsLeftOnHero();
	float NewBulletsNumber = CurrentBulletsLeftInBag + 18;
	Hero->UpdateNumOfBulletsLeftOnHero(NewBulletsNumber);

	AHeroController* HeroController = Cast<AHeroController>(Hero->GetController());
	if (HeroController)
		HeroController->GetInGameHUD()->UpdateNumOfBulletsOnHero(NewBulletsNumber);

	// Destory this actor since it has been picked up
	Destroy();
}

