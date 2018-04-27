// Fill out your copyright notice in the Description page of Project Settings.

#include "EffectsLibrary.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"


UParticleSystem* UEffectsLibrary::BloodImpact_VFX = nullptr;
UParticleSystem* UEffectsLibrary::GenericImpact_VFX = nullptr;
UParticleSystem* UEffectsLibrary::MuzzleFire_VFX = nullptr;
UParticleSystem* UEffectsLibrary::BulletTrail_VFX = nullptr;

UEffectsLibrary::UEffectsLibrary()
{
	// Load all the particles here
	BloodImpact_VFX = FindParticleSystem(TEXT("/Game/Assets/ParticleEffects/BloodImpact/BloodImpact_VFX"));
	GenericImpact_VFX = FindParticleSystem(TEXT("/Game/Assets/ParticleEffects/GenericImpact/GenericImpact_VFX"));
	MuzzleFire_VFX = FindParticleSystem(TEXT("/Game/Assets/ParticleEffects/Muzzle/MuzzleFire_VFX"));
	BulletTrail_VFX = FindParticleSystem(TEXT("/Game/Assets/ParticleEffects/BasicTracer/BulletTrail_VFX"));
}

UParticleSystem * UEffectsLibrary::FindParticleSystem(const TCHAR * _path)
{
	ConstructorHelpers::FObjectFinder<UParticleSystem> ps_finder(_path);
	if (ps_finder.Succeeded())
		return ps_finder.Object;
	else
		return nullptr;
}

UParticleSystem * UEffectsLibrary::GetVFX(EVFXType _vfxType)
{
	switch (_vfxType)
	{
	case EVFXType::BloodImpact:
		return BloodImpact_VFX;

	case EVFXType::GenericImpact:
		return GenericImpact_VFX;

	case EVFXType::MuzzleFire:
		return MuzzleFire_VFX;

	case EVFXType::BulletTrail:
		return BulletTrail_VFX;

	default:
		return nullptr;
	}
}
