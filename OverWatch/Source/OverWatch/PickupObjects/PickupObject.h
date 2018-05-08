// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupObject.generated.h"

UCLASS()
class OVERWATCH_API APickupObject : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* SphereComp;

	
public:	
	// Sets default values for this actor's properties
	APickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
