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
	bool bAllWidgetAreGood = (HealthBar != nullptr) && (HealthValue != nullptr);

	if (!bAllWidgetAreGood)
		return false;

	// Initialize Health Text And Bar
	UpdateHealthValueAndProgressBar(500.0f);

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

