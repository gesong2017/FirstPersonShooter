// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class OVERWATCH_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumOfBulletsOnGun;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumOfBulletsOnHero;

public:
	UInGameHUD(const FObjectInitializer& objInit);

protected:
	bool Initialize() override;
	
public:
	void UpdateHealthValueAndProgressBar(float CurrentHealth);

	void UpdateNumOfBulletsOnGun(int CurrentBullet);

	void UpdateNumOfBulletsOnHero(int CurrentBullet);
};
