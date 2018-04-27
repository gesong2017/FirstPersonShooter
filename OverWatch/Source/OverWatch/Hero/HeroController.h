// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroController.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AHeroController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<class UInGameHUD> InGameHUD_WBP;

	UPROPERTY()
	UInGameHUD* InGameHUD;
	
public:
	AHeroController(const FObjectInitializer & _objectInit);
	
	UFUNCTION()
	void OnPostLogin();

public:

	// FORCEINLINE ACCESSOR
	FORCEINLINE UInGameHUD* GetInGameHUD() const { return InGameHUD; }
	
};
