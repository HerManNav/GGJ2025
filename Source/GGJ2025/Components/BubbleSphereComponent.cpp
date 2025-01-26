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