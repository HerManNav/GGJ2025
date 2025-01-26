

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BubbleBlob.h"
#include "BubbleGameDataAsset.h"
#include "BubbleGameMode.generated.h"


UCLASS()
class GGJ2025_API ABubbleGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Method to register a blob
	void RegisterBlob(ABubbleBlob* Blob);

	// Method to register a blob target
	void RegisterBlobTarget(ABubbleBlobTarget* BlobTarget);

	// Checks whether we won
	UFUNCTION(BlueprintCallable)
	void EvaluateWinCondition();

	// restart current level
	UFUNCTION(BlueprintCallable)
	void RestartCurrentLevel();

	// Check whether an sphereComponent is accounting for score in any bubble blog target
	UFUNCTION(BlueprintCallable, Category = "BubbleBlobs")
	bool IsSphereComponentOverlappingAnyBubbleBlobTarget(const class USphereComponent* sphereComponent) const;

	// Getter for the blobs array, BlueprintCallable
	UFUNCTION(BlueprintCallable, Category = "BubbleBlobs")
	TArray<ABubbleBlob*> GetBubbleBlobs() const;

	// Getter for the blobs array, BlueprintCallable
	UFUNCTION(BlueprintCallable, Category = "BubbleBlobs")
	TArray<class ABubbleBlobTarget*> GetBubbleBlobTargets() const;

protected:
	// Array to hold instances of ABubbleBlob
	UPROPERTY()
	TArray<ABubbleBlob*> BubbleBlobs;

	// Array to hold instances of ABubbleBlob
	UPROPERTY()
	TArray<TObjectPtr<class ABubbleBlobTarget>> BubbleBlobTargets;

public:

	// Property to hold the BubbleGameDataAsset, BlueprintReadOnly
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BubbleGame")
	UBubbleGameDataAsset* BubbleGameDataAsset;
};
