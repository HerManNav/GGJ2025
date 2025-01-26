// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BubbleSphereComponent.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2025_API UBubbleSphereComponent : public USphereComponent
{
	GENERATED_BODY()
	
	//UBubbleSphereComponent();

	//virtual bool AttachToComponent(USceneComponent* InParent, const FAttachmentTransformRules& AttachmentRules, FName InSocketName = NAME_None) override;

public:

	void Clear();

	void AddPotentialCollision(class USphereComponent* sphereComponent);

	void RemovePotentialCollision(class USphereComponent* sphereComponent);

	float GetFillAccuracy();

	bool IsSphereAccountingForOverlap(const class USphereComponent* sphereComponent) const;

private:

	USphereComponent* GetClosestSphereComponent() const;

private:

	UPROPERTY(Transient)
	TArray<TObjectPtr<class USphereComponent>> PotentialCollisions;
};
