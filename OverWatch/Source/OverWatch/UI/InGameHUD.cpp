// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UInGameHUD::UInGameHUD(const FObjectInitializer & objInit) : Super(objInit)
{

}

bool UInGameHUD::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	// Bind Delegetes to Widget components
	bool bAllWidgetAreGood = (HealthBar != nullptr) && (HealthValue != nullptr) && (NumOfBulletsOnGun != nullptr) && (NumOfBulletsOnHero != nullptr);

	if (!bAllWidgetAreGood)
		return false;

	// Initialize Health Text And Bar
	UpdateHealthValueAndProgressBar(500.0f);
	UpdateNumOfBulletsOnGun(36);
	UpdateNumOfBulletsOnHero(72);

	return true;

}

void UInGameHUD::UpdateHealthValueAndProgressBar(float CurrentHealth)
{   
	// Update Health Progress Bar
	float HealthPercentage = CurrentHealth / 500.0f;
	HealthBar->SetPercent(HealthPercentage);

	// Update Health Text Value
	FString HealthText = FString::Printf(TEXT("%d / 500"), (int)CurrentHealth);
	HealthValue->SetText(FText::FromString(HealthText));
}

void UInGameHUD::UpdateNumOfBulletsOnGun(int CurrentBullet)
{
	// Update Number of bullets on gun text Value
	FString NumOfBulletsOnGunText = FString::Printf(TEXT("%d"), CurrentBullet);
	NumOfBulletsOnGun->SetText(FText::FromString(NumOfBulletsOnGunText));
}

void UInGameHUD::UpdateNumOfBulletsOnHero(int CurrentBullet)
{
	// Update Number of bullets on hero text Value
	FString NumOfBulletsOnHeroText = FString::Printf(TEXT("Bullets Left In Bag : %d"), CurrentBullet);
	NumOfBulletsOnHero->SetText(FText::FromString(NumOfBulletsOnHeroText));
}

