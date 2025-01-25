#include "BubbleBlobTarget.h"
#include "Components/BubbleSplineComponent.h"
#include "Components/SphereComponent.h"
#include "BubbleBlob.h"



ABubbleBlobTarget::ABubbleBlobTarget()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the spline component
    SplineComponent = CreateDefaultSubobject<UBubbleSplineComponent>(TEXT("BubbleSplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);
    SplineComponent->SetDrawDebug(true);

    SplineComponent->OnSplineEdited = FSimpleDelegate::CreateUObject(this, &ThisClass::OnSplineEdited);
}

void ABubbleBlobTarget::BeginPlay()
{
    Super::BeginPlay();
    
    GenerateData();
}

void ABubbleBlobTarget::OnBubbleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ABubbleBlob* otherBubble = Cast<ABubbleBlob>(OtherActor))
    {
    }
}

void ABubbleBlobTarget::OnBubbleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ABubbleBlob* otherBubble = Cast<ABubbleBlob>(OtherActor))
    {
    }
}

void ABubbleBlobTarget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABubbleBlobTarget::GenerateData()
{
    if (false == HasAnyFlags(RF_ClassDefaultObject| RF_ArchetypeObject))
    {
        // destroy all the already generated bubble atoms
        ClearBubbleAtoms();

        // if spline component is ok re add the sphere components
        if (ensureAlways(SplineComponent))
        {
            float SplineLength = SplineComponent->GetSplineLength();
            float Distance = 0.0f;

            while (Distance <= SplineLength)
            {
                BubbleTargetDatas.Add(FBubbleTargetData());
                FBubbleTargetData& bubbleTargetData = BubbleTargetDatas.Last();

                FVector SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

                // Create a sphere collision component
                bubbleTargetData.SphereCollision = NewObject<USphereComponent>(this);

                bubbleTargetData.SphereCollision->InitSphereRadius(BeadDiameter / 2.0f);
                bubbleTargetData.SphereCollision->SetWorldLocation(SplineLocation);
                bubbleTargetData.SphereCollision->SetHiddenInGame(false);
                bubbleTargetData.SphereCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
                bubbleTargetData.SphereCollision->RegisterComponent();

                Distance += BeadDiameter;
            }
        }
    }
}

void ABubbleBlobTarget::ClearBubbleAtoms()
{
    for (FBubbleTargetData& bubbleTargetData : BubbleTargetDatas)
    {
        if (::IsValid(bubbleTargetData.SphereCollision))
        {
            bubbleTargetData.SphereCollision->DestroyComponent();
        }

        bubbleTargetData.PotentialCollisions.Empty();
    }

    BubbleTargetDatas.Empty();
}

#if WITH_EDITOR
void ABubbleBlobTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    GenerateData();
}

void ABubbleBlobTarget::PostLoad()
{
    Super::PostLoad();
    
    GenerateData();
}

void ABubbleBlobTarget::OnSplineEdited()
{
    GenerateData();
}

#endif // WITH_EDITOR
