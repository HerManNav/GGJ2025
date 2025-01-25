#include "BubbleBlob.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Console variable to toggle debug draw for bubble atoms
static int32 CVarDebugDrawBubbleAtoms = 1;
FAutoConsoleVariableRef CVarDebugDrawBubbleAtomsRef(
    TEXT("BubbleBlob.DebugDrawBubbleAtoms"),
    CVarDebugDrawBubbleAtoms,
    TEXT("Toggle debug draw for bubble atoms. 0: Off, 1: On"),
    ECVF_Default
);

// Sets default values
ABubbleBlob::ABubbleBlob()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the spline component
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);
    SplineComponent->SetDrawDebug(true);
}

// Called when the game starts or when spawned
void ABubbleBlob::BeginPlay()
{
    Super::BeginPlay();
    EditableSplinePointIndex = 0;
    MakeBubbleAtom();

    EditableSplinePointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
}

// Method to move the second spline point
void ABubbleBlob::MoveBlobEnd(const FVector& NewLocation)
{
    if (EditableSplinePointIndex != INDEX_NONE)
    {
        SplineComponent->SetLocationAtSplinePoint(EditableSplinePointIndex, NewLocation, ESplineCoordinateSpace::World);
    }
}

void ABubbleBlob::MakeBubbleAtom()
{
    if (EditableSplinePointIndex != INDEX_NONE)
    {
        FBubbleAtom NewBubbleAtom;
        NewBubbleAtom.SpawnTime = GetWorld()->GetTimeSeconds();
        NewBubbleAtom.Speed = 0.0f;
        NewBubbleAtom.SplinePointIndex = EditableSplinePointIndex;
        BubbleAtoms.Add(NewBubbleAtom);

        if (OnBlobAtomCreated.IsBound())
        {
            FVector BubbleLocation = SplineComponent->GetLocationAtSplinePoint(NewBubbleAtom.SplinePointIndex, ESplineCoordinateSpace::World);
            OnBlobAtomCreated.Broadcast(BubbleLocation);
        }
    }
}

void ABubbleBlob::SplitBlob()
{
    MakeBubbleAtom();

    if (EditableSplinePointIndex != INDEX_NONE)
    {
        // Get the location of the existing editable spline point
        FVector ExistingPointLocation = SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World);

        // Create a new spline point at the location of the existing editable spline point
        SplineComponent->AddSplineWorldPoint(ExistingPointLocation);

        // Update the EditableSplinePointIndex to the new point
        EditableSplinePointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
    }
}

void ABubbleBlob::CloseBlob()
{
    MakeBubbleAtom();

    EditableSplinePointIndex = INDEX_NONE;

    float SplineLength = SplineComponent->GetSplineLength();
    float Distance = 0.0f;

    while (Distance <= SplineLength)
    {
        FVector SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        // Create a sphere collision component
        USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
        SphereComponent->InitSphereRadius(BeadDiameter / 2.0f);
        SphereComponent->SetWorldLocation(SplineLocation);
        SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        SphereComponent->RegisterComponent();

        Distance += BeadDiameter;
    }

    if (OnBlobClosed.IsBound())
    {
        OnBlobClosed.Broadcast();
    }
}

void ABubbleBlob::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //bool bFreeSpace = true;

    //if (EditableSplinePointIndex != INDEX_NONE)
    //{
    //    FVector EditablePointLocation = SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World);
    //    TArray<FOverlapResult> OverlapResults;
    //    FCollisionShape SphereShape = FCollisionShape::MakeSphere(BeadDiameter / 2.0f);
    //    FCollisionQueryParams QueryParams;

    //    bool bIsOverlapping = GetWorld()->OverlapMultiByChannel(
    //        OverlapResults,
    //        EditablePointLocation,
    //        FQuat::Identity,
    //        ECC_WorldDynamic,
    //        SphereShape,
    //        QueryParams
    //    );

    //    if (bIsOverlapping)
    //    {
    //        for (const FOverlapResult& Result : OverlapResults)
    //        {
    //            if (Result.GetComponent()->IsA<USphereComponent>() && Result.GetComponent()->GetOwner() == this)
    //            {
    //                // Handle overlap with other sphere components belonging to this actor
    //                bFreeSpace = false;
    //                break;
    //            }
    //        }
    //    }
    //}

    //if (bFreeSpace)
    //{   
    //    SplitBlob();
    //}

    if (CVarDebugDrawBubbleAtoms)
    {
        for (const FBubbleAtom& BubbleAtom : BubbleAtoms)
        {
            if (BubbleAtom.SplinePointIndex != INDEX_NONE)
            {
                FVector BubbleLocation = SplineComponent->GetLocationAtSplinePoint(BubbleAtom.SplinePointIndex, ESplineCoordinateSpace::World);
                DrawDebugSphere(GetWorld(), BubbleLocation, BeadDiameter / 2.0f, 12, FColor::Green, false, -1.0f, 0, 1.0f);
                DrawDebugString(GetWorld(), BubbleLocation, FString::FromInt(BubbleAtom.SplinePointIndex), nullptr, FColor::White, DeltaTime, false);
            }
        }

        if (EditableSplinePointIndex != INDEX_NONE)
        {
            FVector EditablePointLocation = SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World);
            DrawDebugSphere(GetWorld(), EditablePointLocation, BeadDiameter / 2.0f, 12, FColor::Red, false, -1.0f, 0, 1.0f);
            DrawDebugString(GetWorld(), EditablePointLocation, FString::FromInt(EditableSplinePointIndex), nullptr, FColor::White, DeltaTime, false);
        }
    }
}
