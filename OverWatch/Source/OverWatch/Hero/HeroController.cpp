// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroController.h"
#include "ConstructorHelpers.h"
#include "UI/InGameHUD.h"

AHeroController::AHeroController(const FObjectInitializer & _objInit) : Super(_objInit)
{
	static ConstructorHelpers::FClassFinder<UInGameHUD> ClassFinder(TEXT("/Game/Blueprints/UI/WBP_InGameHUD"));
	InGameHUD_WBP = ClassFinder.Class;

}

void AHeroController::OnPostLogin()
{   
	// Create In Game HUD
	InGameHUD = CreateWidget<UInGameHUD>(this, InGameHUD_WBP);
	if (InGameHUD)
		InGameHUD->AddToViewport();
}

