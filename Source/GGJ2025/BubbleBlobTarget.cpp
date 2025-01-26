#include "BubbleBlobTarget.h"
#include "Components/BubbleSplineComponent.h"
#include "Components/BubbleSphereComponent.h"
#include "Components/SphereComponent.h"
#include "BubbleBlob.h"
#include "BubbleGameMode.h"

PRAGMA_DISABLE_OPTIMIZATION

ABubbleBlobTarget::ABubbleBlobTarget()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the spline component
    SplineComponent = CreateDefaultSubobject<UBubbleSplineComponent>(TEXT("BubbleSplineComponent"));
    SplineComponent->SetupAttachment(RootComponent);
    SplineComponent->SetDrawDebug(true);

#if WITH_EDITOR
    SplineComponent->OnSplineEdited = FSimpleDelegate::CreateUObject(this, &ThisClass::OnSplineEdited);
#endif
}

void ABubbleBlobTarget::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    GenerateData();
}

void ABubbleBlobTarget::BeginPlay()
{
    Super::BeginPlay();

    // Cache reference to BubbleGameMode
    ABubbleGameMode* GameMode = Cast<ABubbleGameMode>(GetWorld()->GetAuthGameMode());
    if (ensureAlways(GameMode))
    {
        // Successfully casted and cached the reference
        GameMode->RegisterBlobTarget(this);
    }
}

void ABubbleBlobTarget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    ClearBubbleData();
}

void ABubbleBlobTarget::OnBubbleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OverlappedComponent != OtherComp)
    {
        if (ABubbleBlob* otherBubble = Cast<ABubbleBlob>(OtherActor))
        {
            if (UBubbleSphereComponent* bubbleTargetData = Cast<UBubbleSphereComponent>(OverlappedComponent))
            {
                if (USphereComponent* otherSphere = Cast<USphereComponent>(OtherComp))
                {
                    bubbleTargetData->AddPotentialCollision(otherSphere);
                }
            }
        }
    }
}

void ABubbleBlobTarget::OnBubbleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OverlappedComponent != OtherComp)
    {
        if (ABubbleBlob* otherBubble = Cast<ABubbleBlob>(OtherActor))
        {
            if (UBubbleSphereComponent* bubbleTargetData = Cast<UBubbleSphereComponent>(OverlappedComponent))
            {
                if (USphereComponent* otherSphere = Cast<USphereComponent>(OtherComp))
                {
                    bubbleTargetData->RemovePotentialCollision(otherSphere);
                }
            }
        }
    }
}

void ABubbleBlobTarget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float accumFillAccuracy = 0;
    for (UBubbleSphereComponent* bubbleTargetData : BubbleTargetDatas)
    {
        const float fillAccuracy = bubbleTargetData->GetFillAccuracy();
        accumFillAccuracy += fillAccuracy;
    }

    if (0.f < accumFillAccuracy)
    {
        const float finalAccuracy = accumFillAccuracy/static_cast<float>(BubbleTargetDatas.Num());
        UE_LOG(LogTemp, Warning, TEXT("final accuracy = '%f'"), finalAccuracy);
    }
}

bool ABubbleBlobTarget::IsSphereAccounting(const class USphereComponent* sphereComponent) const
{
    bool result = false;
    for (UBubbleSphereComponent* bubbleTargetData : BubbleTargetDatas)
    {
        if (bubbleTargetData->IsSphereAccountingForOverlap(sphereComponent))
        {
            result = true;
            break;
        }
    }

    return result;
}

void ABubbleBlobTarget::GenerateData()
{
    if (false == HasAnyFlags(RF_ClassDefaultObject|RF_ArchetypeObject|RF_Transient))
    {
        // destroy all the already generated bubble atoms
        ClearBubbleData();

        // if spline component is ok re add the sphere components
        if (ensureAlways(SplineComponent))
        {
            float SplineLength = SplineComponent->GetSplineLength();
            float Distance = 0.0f;

            while (Distance <= SplineLength)
            {
                BubbleTargetDatas.Add(NewObject<UBubbleSphereComponent>(this));
                UBubbleSphereComponent* bubbleTargetData = BubbleTargetDatas.Last();

                FVector SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

                // Create a sphere collision component
                bubbleTargetData->InitSphereRadius(BeadDiameter / 2.0f);
                bubbleTargetData->SetWorldLocation(SplineLocation);
                bubbleTargetData->SetHiddenInGame(false);
                bubbleTargetData->SetCollisionProfileName(TEXT("BubbleBlob"));
                bubbleTargetData->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
                bubbleTargetData->SetGenerateOverlapEvents(true);
                bubbleTargetData->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBubbleBeginOverlap);
                bubbleTargetData->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBubbleEndOverlap);
                bubbleTargetData->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
                bubbleTargetData->RegisterComponent();

                Distance += BeadDiameter;
            }
        }
    }
}

void ABubbleBlobTarget::ClearBubbleData()
{
    for (UBubbleSphereComponent* bubbleTargetData : BubbleTargetDatas)
    {
        if (::IsValid(bubbleTargetData))
        {
            bubbleTargetData->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnBubbleBeginOverlap);
            bubbleTargetData->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnBubbleEndOverlap);
            bubbleTargetData->DetachFromParent();
            bubbleTargetData->Clear();
            bubbleTargetData->DestroyComponent();
        }
    }

    BubbleTargetDatas.Empty();
}

#if WITH_EDITOR
void ABubbleBlobTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    if (false == IsRunningGame())
    {
        GenerateData();
    }
}

void ABubbleBlobTarget::OnSplineEdited()
{
    if (false == IsRunningGame())
    {
        GenerateData();
    }
}

#endif // WITH_EDITOR

PRAGMA_ENABLE_OPTIMIZATION
