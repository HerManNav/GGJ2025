
// Fill out your copyright notice in the Description page of Project Settings.

#include "BubbleBlob.h"
#include "Components/SphereComponent.h"
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

    float SplineLength = SplineComponent->GetSplineLength();
    float Distance = 0.0f;

    while (Distance <= SplineLength)
    {
        FVector SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        // Create a sphere collision component
        USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
        SphereComponent->InitSphereRadius(BeadDiameter / 2.0f);
        SphereComponent->SetWorldLocation(SplineLocation);
        SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        SphereComponent->RegisterComponent();

        Distance += BeadDiameter;
    }
}

// Called every frame
void ABubbleBlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
