#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BubbleBlobTarget.generated.h"

UCLASS()
class GGJ2025_API ABubbleBlobTarget : public AActor
{
    GENERATED_BODY()

    ABubbleBlobTarget();

public:

    virtual void OnConstruction(const FTransform& Transform) override;

    // New event for when a bubble atom begins overlap
    UFUNCTION(BlueprintCallable, Category = "Bubble")
    void OnBubbleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // New event for when a bubble atom begins overlap
    UFUNCTION(BlueprintCallable, Category = "Bubble")
    void OnBubbleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool IsSphereAccounting(const class USphereComponent* sphereComponent) const;

    bool IsFill() const;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    // overridden to destroy all the already generated bubble atoms
    void GenerateData();

private:
#if WITH_EDITOR
    void OnSplineEdited();
#endif

    void ClearBubbleData();

    float ComputeFillAccuracy() const;

private:

    // Spline component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class UBubbleSplineComponent* SplineComponent;
    
    UPROPERTY()
    TArray<TObjectPtr<class ABubbleBlob>> CachedBubbles;

    UPROPERTY()
    TArray<TObjectPtr<class USphereComponent>> CachedSphereComponents;

    UPROPERTY()
    TArray<class UBubbleSphereComponent*> BubbleTargetDatas;

    // Bead diameter
    UPROPERTY(EditAnywhere, Category = "Spline")
    float BeadDiameter = 50.f;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowTargetsInGame = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bDebugAccuracy = true;

    // min pct to safisfy this target
    UPROPERTY(EditAnywhere, Category = "Gameplay")
    float SatisfyPct = 0.7f;
};
