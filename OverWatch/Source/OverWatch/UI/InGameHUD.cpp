// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameHUD.h"
#include "ConstructorHelpers.h"
#include "Hero/HeroController.h"

AInGameHUD::AInGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexture(TEXT("/Game/Assets/UI/HUD/Crosshair"));
	Crosshair = UCanvas::MakeIcon(CrosshairTexture.Object, 0.0f, 0.0f, 100.0f, 100.0f);

	//Initialize variables here
	UIScale = 0.0f;
}

void AInGameHUD::DrawHUD()
{   
	//Roughly calculate UI scale by compare Y with 1080
	UIScale = Canvas->ClipY / 1080.0f;

	// Draw all the stuffs on HUD here
	DrawCrosshairOnHUD();
}

void AInGameHUD::DrawCrosshairOnHUD()
{   
	AHeroController* HeroController = Cast<AHeroController>(PlayerOwner);
	if (HeroController)
	{
		float CenterX = Canvas->ClipX*0.5f;
		float CenterY = Canvas->ClipY*0.5f;

		// Draw Crosshair at Center
		float CrosshairIconX = CenterX - Crosshair.UL * UIScale * 0.5f;
		float CrosshairIconY = CenterY - Crosshair.VL * UIScale * 0.5f;
		Canvas->DrawIcon(Crosshair, CrosshairIconX, CrosshairIconY, UIScale);
	}
}

