// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleGameMode.h"

void ABubbleGameMode::RegisterBlob(ABubbleBlob* Blob)
{
    BubbleBlobs.Add(Blob);
}

TArray<ABubbleBlob*> ABubbleGameMode::GetBubbleBlobs() const
{
    return BubbleBlobs;
}
