// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseAICharacter.h"
#include "NormalRangeTrooper.generated.h"

UENUM(BlueprintType)
enum class ERangeTrooperType : uint8
{
	RifleTrooper 				     UMETA(DisplayName = "RifleTrooper"),
	GrenadeTrooper			         UMETA(DisplayName = "GrenadeTrooper")
};

/**
 * 
 */
UCLASS()
class OVERWATCH_API ANormalRangeTrooper : public ABaseAICharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	ERangeTrooperType TrooperType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Behavior Info")
	bool bIsTargetSet;

	/** AI gun classes*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AAIGun* AIGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AAIGun> Rifle_BP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AAIGun> GrenadeLauncher_BP;

public:
	// Sets default values for this character's properties
	ANormalRangeTrooper();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

protected:
	UFUNCTION()
	void OnHealthChanged(UAttributeComponent* AttributeComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser) override;

	UFUNCTION()
	void OnTargetSensed(APawn* Pawn) override;

public :
	void Fire(AActor* TargetActor);
	
};
