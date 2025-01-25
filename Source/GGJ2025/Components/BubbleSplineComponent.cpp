// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleSplineComponent.h"

PRAGMA_DISABLE_OPTIMIZATION

#if WITH_EDITOR
void UBubbleSplineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OnSplineEdited.ExecuteIfBound();
}
#endif

void UBubbleSplineComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

PRAGMA_ENABLE_OPTIMIZATION