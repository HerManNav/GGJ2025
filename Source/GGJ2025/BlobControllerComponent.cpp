#include "BlobControllerComponent.h"
#include "GameFramework/Actor.h"
#include "BubbleBlob.h"
#include "Engine/World.h"
#include "BubbleGameMode.h"

// Sets default values for this component's properties
UBlobControllerComponent::UBlobControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UBlobControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Cache reference to BubbleGameMode
	ABubbleGameMode* GameMode = Cast<ABubbleGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Successfully casted and cached the reference
		BubbleGameMode = GameMode;
	}
	else
	{
		// Handle the case where the cast failed
		UE_LOG(LogTemp, Warning, TEXT("BlobControllerComponent: Failed to cast to ABubbleGameMode"));
	}
}

// Called every frame
void UBlobControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentBlob)
	{
		// Move the blob to the owner's location using MoveBlobToEnd
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		CurrentBlob->MoveBlobEnd(OwnerLocation);
	}
}

// Blueprint callable method to start the blob
void UBlobControllerComponent::SplitBlob()
{
	if (CurrentBlob)
	{
		CurrentBlob->SplitBlob();
	}
}

// Blueprint callable method to close the blob
void UBlobControllerComponent::CloseBlob()
{
	if (CurrentBlob)
	{
		CurrentBlob->CloseBlob();
		CurrentBlob = nullptr;
	}
}

void UBlobControllerComponent::StartBlob()
{
	if (CurrentBlob)
	{
		// If there's already a blob, do nothing
		return;
	}

	// Get the owner actor's location
	FVector OwnerLocation = GetOwner()->GetActorLocation();

	// Spawn the blob at the owner's location
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = BubbleGameMode;
		SpawnParams.Instigator = GetOwner()->GetInstigator();

		CurrentBlob = World->SpawnActor<ABubbleBlob>(BlobClass, OwnerLocation, FRotator::ZeroRotator, SpawnParams);
		if (CurrentBlob && BubbleGameMode)
		{
			BubbleGameMode->RegisterBlob(CurrentBlob);
		}
	}
}