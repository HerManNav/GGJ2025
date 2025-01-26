// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleGameFunctionLibrary.h"
#include "BubbleGameMode.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"

UBubbleGameDataAsset* UBubbleGameFunctionLibrary::GetGameData(const UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    AGameModeBase* GameMode = World->GetAuthGameMode();
    if (!GameMode)
    {
        return nullptr;
    }

    ABubbleGameMode* BubbleGameMode = Cast<ABubbleGameMode>(GameMode);
    if (!BubbleGameMode)
    {
        return nullptr;
    }

    return BubbleGameMode->BubbleGameDataAsset;
}
