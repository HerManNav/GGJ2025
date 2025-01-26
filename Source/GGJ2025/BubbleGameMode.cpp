// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleGameMode.h"
#include "BubbleBlobTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
PRAGMA_DISABLE_OPTIMIZATION
void ABubbleGameMode::RegisterBlob(ABubbleBlob* Blob)
{
    BubbleBlobs.Add(Blob);
}

void ABubbleGameMode::RegisterBlobTarget(ABubbleBlobTarget* BlobTarget)
{
    BubbleBlobTargets.Add(BlobTarget);
}

void ABubbleGameMode::EvaluateWinCondition()
{
    bool bWin = false;

    for (const ABubbleBlobTarget* bubbleBlogTarget : BubbleBlobTargets)
    {
        bWin = bubbleBlogTarget->IsFill();
        if (false == bWin)
        {
            break;
        }
    }

    if (bWin)
    {
        RestartCurrentLevel();
    }
}

void ABubbleGameMode::RestartCurrentLevel()
{
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerControllerFromID(World, 0);
        if (ensureAlways(PC))
        {
            PC->RestartLevel();
        }
    }
}

bool ABubbleGameMode::IsSphereComponentOverlappingAnyBubbleBlobTarget(const class USphereComponent* sphereComponent) const
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
PRAGMA_ENABLE_OPTIMIZATION