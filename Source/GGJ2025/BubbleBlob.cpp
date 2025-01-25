// Fill out your copyright notice in the Description page of Project Settings.

#include "BubbleBlob.h"

#include "Components/SplineComponent.h"

// Sets default values
ABubbleBlob::ABubbleBlob()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the spline component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetDrawDebug(true);
}

// Called when the game starts or when spawned
void ABubbleBlob::BeginPlay()
{
	Super::BeginPlay();

	// Create the first spline point at the actor's spawn location
	//SplineComponent->AddSplineWorldPoint(GetActorLocation());

	// Create the second spline point and store a reference to it
	//FVector SecondPointLocation = GetActorLocation() + BubbleSpawnEndOffset; // Example offset
	//SplineComponent->AddSplineWorldPoint(SecondPointLocation);
	EditableSplinePointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
}

// Method to move the second spline point
void ABubbleBlob::MoveBlobEnd(const FVector& NewLocation)
{
	if (EditableSplinePointIndex != INDEX_NONE)
	{
		SplineComponent->SetLocationAtSplinePoint(EditableSplinePointIndex, NewLocation, ESplineCoordinateSpace::World);
	}
}


void ABubbleBlob::SplitBlob()
{
    if (EditableSplinePointIndex != INDEX_NONE)
    {
        // Get the location of the existing editable spline point
        FVector ExistingPointLocation = SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World);

        // Create a new spline point at the location of the existing editable spline point
        SplineComponent->AddSplineWorldPoint(ExistingPointLocation);

        // Update the EditableSplinePointIndex to the new point
        EditableSplinePointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
    }
}

void ABubbleBlob::CloseBlob()
{
    EditableSplinePointIndex = INDEX_NONE;
}

// Called every frame
void ABubbleBlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


