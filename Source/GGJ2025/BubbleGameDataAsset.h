// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BubbleGameDataAsset.generated.h"

UCLASS()
class GGJ2025_API UBubbleGameDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Time to evaluate a blob
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blob")
	float BlobEvaluationTime = 0.5f;

	// Time after which a bad blob expires
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blob")
	float BadBlobExpirationTime = 3.f;
};
