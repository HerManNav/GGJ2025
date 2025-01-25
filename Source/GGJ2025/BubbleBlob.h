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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bubble")
    float SpawnTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bubble")
    float Speed;

	bool bMoving = true;

	int32 SplinePointIndex = INDEX_NONE;

	float RandomTimeOffset;

	UPROPERTY(Transient)
	USphereComponent* SphereCollision = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlobClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlobAtomCreated, const FVector&, BubbleAtomLocation);

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


private:
	// Spline component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

	// Index of the second spline point
	int32 EditableSplinePointIndex;

	// Bead diameter
	UPROPERTY(EditAnywhere, Category = "Spline")
	float BeadDiameter = 50.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Bubble")
	void MakeBubbleAtom();
	
	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobAtomCreated OnBlobAtomCreated;

	UPROPERTY(BlueprintAssignable, Category = "Bubble")
	FOnBlobClosed OnBlobClosed;

public:
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void MoveBlobEnd(const FVector& NewLocation);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SplitBlob();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void CloseBlob();

	UPROPERTY(BlueprintReadOnly)
	TArray<FBubbleAtom> BubbleAtoms;
};