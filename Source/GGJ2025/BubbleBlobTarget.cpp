#include "BubbleBlobTarget.h"
#include "Components/BubbleSplineComponent.h"
#include "Components/SphereComponent.h"


PRAGMA_DISABLE_OPTIMIZATION

ABubbleBlobTarget::ABubbleBlobTarget()
{
    SplineComponent->OnSplineEdited = FSimpleDelegate::CreateUObject(this, &ThisClass::OnSplineEdited);
}

void ABubbleBlobTarget::BeginPlay()
{
    Super::BeginPlay();
    
    GenerateSpheres();
}

void ABubbleBlobTarget::GenerateSpheres()
{
    if (false == HasAnyFlags(RF_ClassDefaultObject| RF_ArchetypeObject))
    {
        // destroy all the already generated bubble atoms
        ClearBubbleAtoms();

        // if spline component is ok re add the sphere components
        if (ensureAlways(SplineComponent))
        {
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

#if WITH_EDITOR
        UpdateCollisionsVisibility();
#endif
    }
}

void ABubbleBlobTarget::ClearBubbleAtoms()
{
    // Iterate through all components of the actor
    TArray<UActorComponent*> Components = K2_GetComponentsByClass(USphereComponent::StaticClass());

    for (UActorComponent* Component : Components)
    {
        // Check if the component is a primitive component
        if (USphereComponent* SphereComponent = Cast<USphereComponent>(Component))
        {
            // Destroy all existing sphere component
            SphereComponent->DestroyComponent();
        }
    }

    BubbleAtoms.Empty();
}

#if WITH_EDITOR
void ABubbleBlobTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    GenerateSpheres();
}

void ABubbleBlobTarget::PostLoad()
{
    Super::PostLoad();
    
    GenerateSpheres();
}

void ABubbleBlobTarget::OnSplineEdited()
{
    GenerateSpheres();
}

void ABubbleBlobTarget::UpdateCollisionsVisibility()
{
    // Iterate through all components of the actor
    TArray<UActorComponent*> Components = K2_GetComponentsByClass(USphereComponent::StaticClass());
    
    for (UActorComponent* Component : Components)
    {
        // Check if the component is a primitive component
        if (USphereComponent* SphereComponent = Cast<USphereComponent>(Component))
        {
            // Set visibility
            SphereComponent->SetVisibility(true);
        }
    }
}

#endif // WITH_EDITOR


PRAGMA_ENABLE_OPTIMIZATION