// Fill out your copyright notice in the Description page of Project Settings.


#include "BlobMovementComponent.h"
#include "Components/SphereComponent.h"
#include "BubbleBlob.h"

// Sets default values for this component's properties
UBlobMovementComponent::UBlobMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlobMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache the owner as BlobActor
	BlobActor = Cast<ABubbleBlob>(GetOwner());

	// Find and cache USplineComponent
	USplineComponent* SplineComponent = GetOwner()->FindComponentByClass<USplineComponent>();
	if (SplineComponent)
	{
		// Cache the spline component in a member variable
		CachedSplineComponent = SplineComponent;
	}
}


// Called every frame
void UBlobMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CachedSplineComponent && BlobActor && BlobActor->BubbleState != EBubbleState::Blowing)
	{
		// Get the current game time
		float CurrentGameTime = GetWorld()->GetTimeSeconds();

		// Iterate all blob atoms in BlobActor
		for (auto& Atom : BlobActor->BubbleAtoms)
		{

            float TimeOffset = Atom.RandomTimeOffset + CurrentGameTime;
			// Calculate shake offset for Z axis
			float ZOffset = FMath::Sin(TimeOffset / ShakePeriod) * ShakeDistance;

			// Calculate shake offset for X axis 
			float XOffset = FMath::Cos(TimeOffset / ShakePeriod) * ShakeDistance;

			// Apply the shake offset to the Actor's location
			FVector Offset = FVector(XOffset, 0.0f, ZOffset);


			// Calculate the lifetime of the atom
			float AtomLifetime = CurrentGameTime - BlobActor->BubbleAtoms[0].SpawnTime;

			// Do something with AtomLifetime if needed

            if (BubbleAtomAcceleration)
            {
                // Get the acceleration value from the curve
                float Acceleration = BubbleAtomAcceleration->GetFloatValue(AtomLifetime);
                // Do something with Acceleration if needed

                // Update the speed of the atom
                Atom.Speed += Acceleration * DeltaTime;

                // Get the location of the atom
                FVector AtomLocation = CachedSplineComponent->GetLocationAtSplinePoint(Atom.SplinePointIndex, ESplineCoordinateSpace::World);                
                AtomLocation += Offset;
				if (Atom.bMoving)
				{
					AtomLocation += BubbleFloatDirection * Atom.Speed * DeltaTime;
				}

				auto PrevTangent = CachedSplineComponent->GetTangentAtSplinePoint(Atom.SplinePointIndex, ESplineCoordinateSpace::World);
				CachedSplineComponent->SetLocationAtSplinePoint(Atom.SplinePointIndex, AtomLocation, ESplineCoordinateSpace::World);
				CachedSplineComponent->SetTangentAtSplinePoint(Atom.SplinePointIndex, PrevTangent, ESplineCoordinateSpace::World);

                Atom.SphereCollision->SetWorldLocation(AtomLocation);
            }	
		}
	}
}

