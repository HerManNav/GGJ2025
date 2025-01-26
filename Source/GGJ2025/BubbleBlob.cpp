#include "BubbleBlob.h"
#include "BubbleBlobTarget.h"
#include "Components/SphereComponent.h"
#include "Components/BubbleSplineComponent.h"
#include "BubbleGameFunctionLibrary.h"
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

    // Initialize the root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Initialize the spline component
    SplineComponent = CreateDefaultSubobject<UBubbleSplineComponent>(TEXT("BubbleSplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);
    SplineComponent->SetDrawDebug(true);
}

// Called when the game starts or when spawned
void ABubbleBlob::BeginPlay()
{
    Super::BeginPlay();
    EditableSplinePointIndex = 0;
    MakeBubbleAtom();
    BubbleState = EBubbleState::Blowing;
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

        // Assign a random number to RandomTimeOffset property
        NewBubbleAtom.RandomTimeOffset = FMath::RandRange(0.0f, 1.0f);

        FVector BubbleLocation = SplineComponent->GetLocationAtSplinePoint(NewBubbleAtom.SplinePointIndex, ESplineCoordinateSpace::World);

        USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
        SphereComponent->InitSphereRadius(BeadDiameter / 2.0f);
        SphereComponent->SetWorldLocation(BubbleLocation);
        SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
        SphereComponent->RegisterComponent();
        SphereComponent->SetCollisionProfileName(TEXT("BubbleBlob"));
        SphereComponent->SetGenerateOverlapEvents(true);
        SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABubbleBlob::OnBubbleAtomBeginOverlap); // Add callback to begin overlap event
        NewBubbleAtom.SphereCollision = SphereComponent;

        BubbleAtoms.Add(NewBubbleAtom);

        if (OnBlobAtomCreated.IsBound())
        {
            OnBlobAtomCreated.Broadcast(BubbleLocation);
        }
    }
}

void ABubbleBlob::OnBubbleAtomBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (BubbleState >= EBubbleState::Locked)
    {
        return;
    }

    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->IsA<ABubbleBlobTarget>())
        {
            return;
        }

        CloseBlob();
        if (BubbleState < EBubbleState::Locked)
        {
            OnBlobStuck.Broadcast();
        }
        if (false == OtherActor->IsA<ABubbleBlob>())
        {
            // Handle overlap with other actors
            StopAndLockAtoms();

            // Iterate linked blobs and set their atoms' bMoving to false
            for (ABubbleBlob* LinkedBlob : LinkedBlobs)
            {
                LinkedBlob->StopAndLockAtoms();
            }
        }
        else
        {
            ABubbleBlob* OtherBubbleBlob = Cast<ABubbleBlob>(OtherActor);
            if (OtherBubbleBlob)
            {
                OtherBubbleBlob->LinkedBlobs.AddUnique(this);
                LinkedBlobs.AddUnique(OtherBubbleBlob);

                if (bLocked || OtherBubbleBlob->bLocked)
                {
                    StopAndLockAtoms();
                    for (ABubbleBlob* LinkedBlob : LinkedBlobs)
                    {
                        LinkedBlob->StopAndLockAtoms();
                    }
                }
            }
        }
    }
}

void ABubbleBlob::StopAndLockAtoms()
{
    bLocked = true;
    BubbleState = EBubbleState::Locked;
    LockedInTime = 0.f;
    for (FBubbleAtom& BubbleAtom : BubbleAtoms)
    {
        BubbleAtom.bMoving = false;
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
    if (BubbleState == EBubbleState::Blowing)
    {
        MakeBubbleAtom();
        EditableSplinePointIndex = INDEX_NONE;
        BubbleState = EBubbleState::Closed;
        if (OnBlobClosed.IsBound())
        {
            OnBlobClosed.Broadcast();
        }
    }
    
}

void ABubbleBlob::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DrawDebugBubbles(DeltaTime);

    switch (BubbleState)
    {
    case EBubbleState::Blowing:
        if (CanSpawnAtom())
        {
            SplitBlob();
        }
        break;
    case EBubbleState::Locked:
        UpdateLocked(DeltaTime);
        break;
    case EBubbleState::GoodBlob:
        UpdateGood(DeltaTime);
        break;
    case EBubbleState::BadBlob:
        UpdateBad(DeltaTime);
        break;
    }

}

void ABubbleBlob::UpdateLocked(float DeltaTime)
{
    // check for pop condition
    LockedInTime += DeltaTime;

    auto* GameData = UBubbleGameFunctionLibrary::GetGameData(this);
    if (GameData == nullptr)
    {
        return;
    }
    auto* GameMode = Cast<ABubbleGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode == nullptr)
    {
        return;
    }
    if (LockedInTime > GameData->BlobEvaluationTime)
    {
        bool IsGoodBlob = true;
        for (FBubbleAtom& BubbleAtom : BubbleAtoms)
        {
            BubbleAtom.IsAtomGood = (GameMode->IsSphereComponentValidForAnyBubbleBlobTarget(BubbleAtom.SphereCollision));
            IsGoodBlob &= BubbleAtom.IsAtomGood;
        }
        OnBlobEvaluation.Broadcast(BubbleAtoms);
        
        BubbleState = IsGoodBlob ? EBubbleState::GoodBlob : EBubbleState::BadBlob;
        EvaluationTimeStamp = GetWorld()->GetTimeSeconds();
    }
}

void ABubbleBlob::UpdateGood(float DeltaTime)
{
}

void ABubbleBlob::UpdateBad(float DeltaTime)
{
    float TimeSinceEvaluation = GetWorld()->GetTimeSeconds() - EvaluationTimeStamp;
    auto* GameData = UBubbleGameFunctionLibrary::GetGameData(this);
    if (GameData == nullptr)
    {
        return;
    }

    if (TimeSinceEvaluation > GameData->BadBlobExpirationTime)
    {
        BubbleState = EBubbleState::Dead;
        OnBlobExplosion.Broadcast(BubbleAtoms);
    }
}


void ABubbleBlob::DrawDebugBubbles(float DeltaTime)
{
    if (CVarDebugDrawBubbleAtoms)
    {
        FColor BubbleColor;
        FString BubbleStateText;

        switch (BubbleState)
        {
        case EBubbleState::Blowing:
            BubbleColor = FColor::Blue;
            BubbleStateText = TEXT("Blowing");
            break;
        case EBubbleState::Closed:
            BubbleColor = FColor::Black;
            BubbleStateText = TEXT("Closed");
            break;
        case EBubbleState::Locked:
            BubbleColor = FColor::Yellow;
            BubbleStateText = TEXT("Locked");
            break;
        case EBubbleState::GoodBlob:
            BubbleColor = FColor::Green;
            BubbleStateText = TEXT("GoodBlob");
            break;
        case EBubbleState::BadBlob:
            BubbleColor = FColor::Red;
            BubbleStateText = TEXT("BadBlob");
            break;
        case EBubbleState::Dead:
            BubbleColor = FColorList::DustyRose;
            BubbleStateText = TEXT("Dead");
            break;
        default:
            BubbleColor = FColor::White;
            BubbleStateText = TEXT("Unknown");
            break;
        }

        for (const FBubbleAtom& BubbleAtom : BubbleAtoms)
        {
            if (BubbleAtom.SplinePointIndex != INDEX_NONE)
            {
                FVector BubbleLocation = SplineComponent->GetLocationAtSplinePoint(BubbleAtom.SplinePointIndex, ESplineCoordinateSpace::World);
                DrawDebugSphere(GetWorld(), BubbleLocation, BeadDiameter / 2.0f, 12, BubbleColor, false, -1.0f, 100, 2.0f);
                DrawDebugString(GetWorld(), BubbleLocation, FString::FromInt(BubbleAtom.SplinePointIndex) + TEXT(" - ") + BubbleStateText, nullptr, FColor::White, DeltaTime, false);
            }
        }

        if (EditableSplinePointIndex != INDEX_NONE)
        {
            DrawDebugSphere(GetWorld(), SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World), BeadDiameter / 2.0f, 12, FColor::Red, false, -1.0f, 100, 2.0f);
            DrawDebugString(GetWorld(), SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World), FString::FromInt(EditableSplinePointIndex) + TEXT(" - ") + BubbleStateText, nullptr, FColor::White, DeltaTime, false);
        }
    }
}

bool ABubbleBlob::CanSpawnAtom()
{
    bool bFreeSpace = true;

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape SphereShape = FCollisionShape::MakeSphere(BeadDiameter / 2.0f);
    FCollisionQueryParams QueryParams;
    QueryParams.bFindInitialOverlaps = true;


    bool bIsOverlapping = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        SplineComponent->GetLocationAtSplinePoint(EditableSplinePointIndex, ESplineCoordinateSpace::World),
        FQuat::Identity,
        //ECC_GameTraceChannel1,
        ECC_WorldDynamic,
        SphereShape,
        QueryParams
    );

    // Draw a sphere indicating whether overlapping hit something or not
    FColor SphereColor = FColor::Blue;

    if (bIsOverlapping)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {

            if (Result.GetComponent()->IsA<USphereComponent>() && Result.GetComponent()->GetOwner() == this)
            {
                // Handle overlap with other sphere components belonging to this actor
                bFreeSpace = false;
                SphereColor = FColor::Yellow;
                break;
            }
        }
    }
    return bFreeSpace;
}