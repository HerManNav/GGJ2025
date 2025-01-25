#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "BubbleBlob.generated.h"

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

private:
	// Spline component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

	// Index of the second spline point
	int32 EditableSplinePointIndex;

	// Bead diameter
	UPROPERTY(EditAnywhere, Category = "Spline")
	float BeadDiameter = 30.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void MoveBlobEnd(const FVector& NewLocation);

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void SplitBlob();

	UFUNCTION(BlueprintCallable, Category = "Spline")
	void CloseBlob();
};