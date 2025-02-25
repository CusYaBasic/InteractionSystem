// Copyright 2025 - Lewis Herbert


#include "Components/InteractionComponent.h"

DEFINE_LOG_CATEGORY(InteractionDebug);


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::HandleDebug(ELogVerbosity::Type LogType, FString Message)
{
	FString lt = LogType.ToString();
	if (bPrintDebug)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
	else
		UE_LOG(InteractionDebug, Error, TEXT("%s"), Message);
}

bool UInteractionComponent::SetTargetActor(AActor* NewTarget)
{
	if (!IsValid(NewTarget))
	{
		HandleDebug(ELogVerbosity::Error, "UInteractionComponent::SetTargetActor: Target is not valid");
		return false;
	}

	if (!NewTarget->Implements<UInteractionInterface>())
	{
		HandleDebug(ELogVerbosity::Error, "UInteractionComponent::SetTargetActor: Target does not implement 'UInteractionInterface'");
		return false;
	}

}

