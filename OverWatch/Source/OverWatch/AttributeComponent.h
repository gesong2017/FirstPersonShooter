// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

// On Health Changed Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UAttributeComponent*, AttributeComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, IntigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Attributes")
	float MaxiumHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Attributes")
	float BaseDamage;

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleAnyTakenDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* IntigatedBy, AActor* DamageCauser);

public:	
	FORCEINLINE void SetBaseDamage(float Damage) { BaseDamage = Damage; }
	FORCEINLINE void SetCurrentHealth(float Health) { CurrentHealth = Health; }
	FORCEINLINE float GetBaseDamage() const { return BaseDamage; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
};
