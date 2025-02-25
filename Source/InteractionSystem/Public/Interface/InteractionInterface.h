// Copyright 2025 - Lewis Herbert

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for objects that can be interacted with, become targets, and be forgotten as targets.
 */
class INTERACTIONSYSTEM_API IInteractionInterface
{
	GENERATED_BODY()

public:

	// Function for interacting with the object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	// Function to make this object a target
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void BecomeTarget();

	// Function to forget this object as a target
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ForgottenAsTarget();
};
