// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroController.h"
#include "Hero/HeroCameraManager.h"

AHeroController::AHeroController()
{
	PlayerCameraManagerClass = AHeroCameraManager::StaticClass();
}

