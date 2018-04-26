// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FCanvasIcon Crosshair;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	float UIScale;

public:
	AInGameHUD();
	
	virtual void DrawHUD() override;

private:
	void DrawCrosshairOnHUD();
};
