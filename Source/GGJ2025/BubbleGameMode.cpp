// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleGameMode.h"
#include "BubbleBlobTarget.h"

void ABubbleGameMode::RegisterBlob(ABubbleBlob* Blob)
{
    BubbleBlobs.Add(Blob);
}

void ABubbleGameMode::RegisterBlobTarget(ABubbleBlobTarget* BlobTarget)
{
    BubbleBlobTargets.Add(BlobTarget);
}

bool ABubbleGameMode::IsSphereComponentValidForAnyBubbleBlobTarget(const class USphereComponent* sphereComponent) const
{
    bool result = false;
    for (const ABubbleBlobTarget* bubbleBlogTarget : BubbleBlobTargets)
    {
        if (bubbleBlogTarget->IsSphereAccounting(sphereComponent))
        {
            result = true;
            break;
        }
    }

    return result;
}

TArray<ABubbleBlob*> ABubbleGameMode::GetBubbleBlobs() const
{
    return BubbleBlobs;
}

TArray<class ABubbleBlobTarget*> ABubbleGameMode::GetBubbleBlobTargets() const
{
    return BubbleBlobTargets;
}
