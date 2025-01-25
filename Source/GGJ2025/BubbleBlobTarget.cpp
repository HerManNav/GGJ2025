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
    
    CloseBlob();
}

void ABubbleBlobTarget::CloseBlob()
{
    if (false == HasAnyFlags(RF_ClassDefaultObject| RF_ArchetypeObject))
    {
        // destroy all the already generated bubble atoms
        ClearBubbleAtoms();

        Super::CloseBlob();

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
    
    CloseBlob();
}

void ABubbleBlobTarget::PostLoad()
{
    Super::PostLoad();
    
    CloseBlob();
}

void ABubbleBlobTarget::OnSplineEdited()
{
    CloseBlob();
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