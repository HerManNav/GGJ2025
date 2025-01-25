#pragma once

#include "CoreMinimal.h"
#include "BubbleBlob.h"
#include "BubbleBlobTarget.generated.h"

UCLASS()
class GGJ2025_API ABubbleBlobTarget : public ABubbleBlob
{
    GENERATED_BODY()

    ABubbleBlobTarget();

public:

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void PostLoad() override;
#endif // WITH_EDITOR

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // overridden to destroy all the already generated bubble atoms
    void GenerateSpheres();

private:
#if WITH_EDITOR
    void OnSplineEdited();
    void UpdateCollisionsVisibility();
#endif

    void ClearBubbleAtoms();
};
