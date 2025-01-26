#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BubbleGameMode.h"
#include "BubbleGameDataAsset.h"
#include "BubbleGameFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GGJ2025_API UBubbleGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
	static UBubbleGameDataAsset* GetGameData(const UObject* WorldContextObject);
};
