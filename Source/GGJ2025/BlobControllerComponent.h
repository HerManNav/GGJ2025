#pragma once

#include "BubbleBlob.h"
#include "BubbleGameMode.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "BlobControllerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GGJ2025_API UBlobControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlobControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Blueprint callable method to start the blob
	UFUNCTION(BlueprintCallable, Category="Blob")
	void StartBlob();

	// Blueprint callable method to split the blob
	UFUNCTION(BlueprintCallable, Category="Blob")
	void SplitBlob();

	// Blueprint callable method to close the blob
	UFUNCTION(BlueprintCallable, Category="Blob")
	void CloseBlob();

private:
	// Pointer to the current blob instance
	class ABubbleBlob* CurrentBlob = nullptr;

	// Transient reference to the BubbleGameMode
	UPROPERTY(Transient)
	class ABubbleGameMode* BubbleGameMode = nullptr;
};
