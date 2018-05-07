// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthValue;

public:
	UInGameHUD(const FObjectInitializer& objInit);

protected:
	bool Initialize() override;
	
public:
	void UpdateHealthValueAndProgressBar(float CurrentHealth);
};
