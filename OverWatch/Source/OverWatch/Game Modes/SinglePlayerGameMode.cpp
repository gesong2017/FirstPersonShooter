// Fill out your copyright notice in the Description page of Project Settings.

#include "SinglePlayerGameMode.h"
#include "ConstructorHelpers.h"
#include "Hero/Hero.h"
#include "Hero/HeroController.h"
#include "Hero/HeroState.h"
#include "Hero/HeroSpectatorPawn.h"
#include "Online/OverWatchGameState.h"
#include "UI/InGameHUD.h"

ASinglePlayerGameMode::ASinglePlayerGameMode()
{   
	// Find hero class and assign that observer to default pawn class
	static ConstructorHelpers::FClassFinder<AHero> HeroPawnOB(TEXT("/Game/Blueprints/Hero/Hero_BP"));
	DefaultPawnClass = HeroPawnOB.Class;

	// Assign default HUD to in game HUD
	HUDClass = AInGameHUD::StaticClass();

	// Assign default player controller to the hero controller 
	PlayerControllerClass = AHeroController::StaticClass();

	// Assign default player state to hero state
	PlayerStateClass = AHeroState::StaticClass();

	//// Assign default game state to overwatch game state
	//GameStateClass = AOverWatchGameState::StaticClass();

	//// Assign default spectator pawn to hero spectator pawn
	//SpectatorClass = AHeroSpectatorPawn::StaticClass();
}


