// Fill out your copyright notice in the Description page of Project Settings.

#include "SinglePlayerGameMode.h"
#include "ConstructorHelpers.h"
#include "Hero/Hero.h"
#include "Hero/HeroController.h"

ASinglePlayerGameMode::ASinglePlayerGameMode()
{   
	// Find hero class and assign that observer to default pawn class
	static ConstructorHelpers::FClassFinder<AHero> HeroPawnOB(TEXT("/Game/Blueprints/Hero/Hero_BP"));
	DefaultPawnClass = HeroPawnOB.Class;

	// Assign default player controller to the hero controller 
	PlayerControllerClass = AHeroController::StaticClass();
}

void ASinglePlayerGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AHeroController* HeroController = Cast<AHeroController>(NewPlayer);
	if(HeroController)
		HeroController->OnPostLogin();
}
