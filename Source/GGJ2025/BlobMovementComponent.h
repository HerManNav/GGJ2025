
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "BlobMovementComponent.generated.h"
UCLASS( BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJ2025_API UBlobMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlobMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Cached spline component
	UPROPERTY(Transient)
	USplineComponent* CachedSplineComponent;
   
	// Reference to Blob Actor
	UPROPERTY(Transient)
	class ABubbleBlob* BlobActor;

public:
	// Curve float object reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UCurveFloat* BubbleAtomAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector BubbleFloatDirection = FVector::UpVector;
};
