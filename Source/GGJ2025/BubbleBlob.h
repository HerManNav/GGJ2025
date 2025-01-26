#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "BubbleBlob.generated.h"


USTRUCT(BlueprintType)
struct FBubbleAtom
{
	GENERATED_BODY()

	float SpawnTime;
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bubble")
	bool IsAtomGood;

	bool bMoving = true;

	int32 SplinePointIndex = INDEX_NONE;

	float RandomTimeOffset;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereCollision = nullptr;
};


UENUM(BlueprintType)
enum class EBubbleState : uint8
{
    Blowing,
	Closed,
    Locked,
    GoodBlob,
	BadBlob,
	Dead,
	Dormant
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlobClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlobAtomCreated, const FVector&, BubbleAtomLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlobStuck);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlobEvaluation, const TArray<FBubbleAtom>&, BubbleAtoms);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlobExplosion, const TArray<FBubbleAtom>&, BubbleAtoms);

UCLASS()
class GGJ2025_API ABubbleBlob : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	FVector BubbleSpawnEndOffset = FVector(2.0f, 0.f, 0.f);

	// Sets default values for this actor's properties
	ABubbleBlob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// New event for when a bubble atom begins overlap
	UFUNCTION(BlueprintCallable, Category = "Bubble")
	void OnBubbleAtomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StopAndLockAtoms();

private:
	// Spline component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBubbleSplineComponent* SplineComponent;

	// Bead diameter
	UPROPERTY(EditAnywhere, Category = "Spline")
	float BeadDiameter = 100.f;

	// Index of the second spline point
	int32 EditableSplinePointIndex;

	void DrawDebugBubbles(float DeltaTime);

	bool CanSpawnAtom();

	void UpdateLocked(float DeltaTime);
	void UpdateGood(float DeltaTime);
	void UpdateBad(float DeltaTime);


public:
	UFUNCTION(BlueprintCallable, Category = "Bubble")
	void MakeBubbleAtom();

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobAtomCreated OnBlobAtomCreated;

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobClosed OnBlobClosed;

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobStuck OnBlobStuck;

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobEvaluation OnBlobEvaluation;

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobExplosion OnBlobExplosion;

public:
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void MoveBlobEnd(const FVector& NewLocation);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SplitBlob();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void CloseBlob();

	UPROPERTY(BlueprintReadOnly)
	TArray<FBubbleAtom> BubbleAtoms;

	UPROPERTY(Transient)
	TArray<ABubbleBlob*> LinkedBlobs;

	bool bLocked = false;
	float LockedInTime;
	float EvaluationTimeStamp;

	EBubbleState BubbleState = EBubbleState::Blowing;
};