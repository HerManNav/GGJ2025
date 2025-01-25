// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "BubbleSplineComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class GGJ2025_API UBubbleSplineComponent : public USplineComponent
{
	GENERATED_BODY()
	
public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginDestroy() override;

public:
	// Triggered when the spline is edited
	FSimpleDelegate OnSplineEdited;
};
