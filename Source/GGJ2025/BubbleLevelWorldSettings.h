// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "BubbleLevelWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2025_API ABubbleLevelWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
    // The level to load when some condition is met.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Loading", meta = (AllowedClasses = "/Script/Engine.World"))
    TSoftObjectPtr<UWorld> NextLevelToLoad;
};
