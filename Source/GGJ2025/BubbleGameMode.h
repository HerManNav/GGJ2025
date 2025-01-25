#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BubbleBlob.h"
#include "BubbleGameMode.generated.h"


UCLASS()
class GGJ2025_API ABubbleGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Method to register a blob
	void RegisterBlob(ABubbleBlob* Blob);

	// Getter for the blobs array, BlueprintCallable
	UFUNCTION(BlueprintCallable, Category = "BubbleBlobs")
	TArray<ABubbleBlob*> GetBubbleBlobs() const;

protected:
	// Array to hold instances of ABubbleBlob
	UPROPERTY()
	TArray<ABubbleBlob*> BubbleBlobs;
};
