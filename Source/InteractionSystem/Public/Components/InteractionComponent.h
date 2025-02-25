// Copyright 2025 - Lewis Herbert

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logging/LogMacros.h"
#include "Interface/InteractionInterface.h"
#include "InteractionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(InteractionDebug, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	UPROPERTY(BlueprintReadWrite, Category = "Interact|Checks")
	AActor* TargetActor;

protected:

	// Should we check for interactables on BeginPlay()?
	UPROPERTY(BlueprintReadWrite, Category = "Interact|Checks")
	bool bStartOnBeginPlay = false;
	// Should we print our debug message to the screen?
	UPROPERTY(BlueprintReadWrite, Category = "Interact|Checks")
	bool bPrintDebug = false;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Handle Debug
	 *
	 * Checks if 'bPrintDebug' is true and prints our debug message to the screen if so. If not; only prints to the log.
	 *
	 * @param Message Message to print to the screen/log
	 */
	void HandleDebug(ELogVerbosity::Type LogType, FString Message);
	/**
	 * @brief Set Target Actor
	 *
	 * This function checks if our 'NewTarget' is valid and implements 'UInteractionInterface'.
	 * Then checks if our 'NewTarget' is not equal to our 'TargetActor',
	 * Sets the actor if all conditions are met.
	 * Finally fires delegates to the respected actors involved.
	 *
	 * @param NewTarget Reference to the new target actor
	 * @return If the actor was successfully updated
	 */
	bool SetTargetActor(AActor* NewTarget);
};
