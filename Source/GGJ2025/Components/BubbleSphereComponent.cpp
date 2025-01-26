// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleSphereComponent.h"

/*UBubbleSphereComponent::UBubbleSphereComponent()
{
   UE_LOG(LogTemp, Warning, TEXT(" ---- constructed bubble sphere component '%s'"), *GetName());
}

bool UBubbleSphereComponent::AttachToComponent(USceneComponent* InParent, const FAttachmentTransformRules& AttachmentRules, FName InSocketName)
{
    return Super::AttachToComponent(InParent, AttachmentRules, InSocketName);
}*/


void UBubbleSphereComponent::Clear()
{
    PotentialCollisions.Empty();
}

void UBubbleSphereComponent::AddPotentialCollision(class USphereComponent* sphereComponent)
{
    PotentialCollisions.Add(sphereComponent);
}

void UBubbleSphereComponent::RemovePotentialCollision(class USphereComponent* sphereComponent)
{
    PotentialCollisions.Remove(sphereComponent);
}

float UBubbleSphereComponent::GetFillAccuracy()
{
    float result = 0.f;

    if (USphereComponent* closestSphereComponent = GetClosestSphereComponent())
    {
        const float radius = GetScaledSphereRadius();
        const float radiusSq = radius * radius;

        FVector center = GetComponentLocation();
        center.Y = 0.f;
        float minDist2DSq = FLT_MAX;
        
        FVector otherCenter = closestSphereComponent->GetComponentLocation();
        otherCenter.Y = 0.f;

        const float dist2DSq = (FVector::DistSquared(center, otherCenter) * 0.5f);

        result = (radiusSq - dist2DSq)/radiusSq;
    }

    return result;
}

USphereComponent* UBubbleSphereComponent::GetClosestSphereComponent() const
{
    USphereComponent* result = nullptr;

    if (0 < PotentialCollisions.Num())
    {
        FVector center = GetComponentLocation();
        center.Y = 0.f;
        float minDist2DSq = FLT_MAX;
        for (USphereComponent* sphereComponent : PotentialCollisions)
        {
            FVector otherCenter = sphereComponent->GetComponentLocation();
            otherCenter.Y = 0.f;
            
            const float dist2DSq = FVector::DistSquared(center, otherCenter);
            if (dist2DSq < minDist2DSq)
            {
                minDist2DSq = dist2DSq;
                result = sphereComponent;
            }
        }
    }

    return result;
}

bool UBubbleSphereComponent::IsSphereAccountingForOverlap(USphereComponent* sphereComponent) const
{
    bool result = false;
    
    if (const USphereComponent* closestSphereComponent = GetClosestSphereComponent())
    {
        result = (closestSphereComponent->GetName() == sphereComponent->GetName());
    }

    return result;
}
