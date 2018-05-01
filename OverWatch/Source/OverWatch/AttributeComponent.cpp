// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	MaxiumHealth = 0.0f;
	CurrentHealth = 0.0f;
	BaseDamage = 0.0f;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind Damage Taken Event
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UAttributeComponent::HandleAnyTakenDamage);

	CurrentHealth = MaxiumHealth;
}

void UAttributeComponent::HandleAnyTakenDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * IntigatedBy, AActor * DamageCauser)
{   
	// if damage is less than or equal to 0, return
	if (Damage <= 0.0f)
		return;

	//else
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxiumHealth);

	// call our custom Delegate 
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, IntigatedBy, DamageCauser);
}


