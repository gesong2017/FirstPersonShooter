// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectsLibrary.generated.h"

UENUM()
enum class EVFXType : uint8
{
	BloodImpact = 0,
	GenericImpact,
	MuzzleFire,
	BulletTrail
};

class UParticleSystem;
class USoundCue;

/**
 * 
 */
UCLASS()
class OVERWATCH_API UEffectsLibrary : public UObject
{
	GENERATED_BODY()
	
private:

	// All the particles
	static UParticleSystem* BloodImpact_VFX;

	static UParticleSystem* GenericImpact_VFX;

	static UParticleSystem* MuzzleFire_VFX;

	static UParticleSystem* BulletTrail_VFX;

public:
	UEffectsLibrary();

private:
	UParticleSystem * FindParticleSystem(const TCHAR* _path);

public:
	static UParticleSystem* GetVFX(EVFXType _vfxType);
	
	
};
