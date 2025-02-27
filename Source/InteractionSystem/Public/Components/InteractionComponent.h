// Copyright 2025 - Lewis Herbert

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logging/LogMacros.h"
#include "Interface/InteractionInterface.h"
#include "InteractionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(InteractionDebug, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorUpdated, AActor*, NewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetActorForgotten, AActor*, OldTarget);

UENUM(Blueprintable)
enum EInteractionLogType : uint8
{
	Log UMETA(DisplayName = "Log"),
	Warning UMETA(DisplayName = "Warning"),
	Error UMETA(DisplayName = "Error"),
	Success UMETA(DisplayName = "Success")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	// Delegate that fires when TargetActor is set or forgotten
	UPROPERTY(BlueprintAssignable, Category = "Interact|Bind")
	FOnTargetActorUpdated OnTargetActorUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Interact|Bind")
	FOnTargetActorForgotten OnTargetActorForgotten;
	// Current interactable target actor
	UPROPERTY(BlueprintReadWrite, Category = "Interact|Checks")
	AActor* TargetActor;

protected:

	// Should we check for interactables on BeginPlay()?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Checks")
	bool bStartOnBeginPlay = false;
	// Should we print our debug message to the screen?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Checks")
	bool bPrintDebug = false;
	// Should we print our debug trace to the screen?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Checks")
	bool bPrintDebugTrace = false;
	// Distance in units of how far we should trace
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Data")
	float InteractionRange = 1500.0f;
	// How fast in secs should we re-trace
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Data")
	float InteractionRate = 0.1f;
	// Should we add a offset to the start of the trace?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact|Data")
	float InteractionStartOffset = 0.0f;

	FTimerHandle TraceTimerHandle; // Timer for repeated traces

	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/**
	 * @brief Preform Interaction Trace
	 *
	 * Checks if a timer is not running, if our owner is a character and that it has a valid player controller.
	 * Then if all conditions are met draws a trace with the specified params
	 * Updates TargetActor accordingly
	 */
	void PerformInteractionTrace();
	/**
	 * @brief Handle Debug
	 *
	 * Checks if 'bPrintDebug' is true and prints our debug message to the screen if so. If not; only prints to the log.
	 *
	 * @param Message Message to print to the screen/log
	 */
	void HandleDebug(EInteractionLogType InLogType, FString Message);
	/**
	 * @brief Set Target Actor
	 *
	 * This function checks if our 'NewTarget' is valid and implements 'UInteractionInterface'.
	 * Then checks if our 'NewTarget' is not equal to our 'TargetActor',
	 * Sets the actor if all conditions are met.
	 * Finally fires delegates to the respected actors involved.
	 *
	 * @param NewTarget Reference to the new target actor.
	 * @return If the actor was successfully updated.
	 */
	bool SetTargetActor(AActor* NewTarget);
	/**
	 * @brief Clear Target Actor
	 *
	 * Clears our current target actor.
	 * 
	 * @return If the actor was successfully cleared.
	 */
	bool ClearTargetActor();

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Overides for clean up
	virtual void Deactivate() override;
	virtual void OnUnregister() override;

	/**
	 * @brief Start Interaction Trace
	 *
	 * Starts our interaction trace for interactable world objects.
	 */
	UFUNCTION(Blueprintcallable, Category = "Interact|Trace")
	void StartInteractionTrace();
	/**
	 * @brief Stop Interaction Trace
	 *
	 * Stops our interaction trace for interactable world objects.
	 */
	UFUNCTION(Blueprintcallable, Category = "Interact|Trace")
	void StopInteractionTrace();
	/**
	 * @brief Is Running
	 *
	 * Checks if we have a interaction trace currently running.
	 *
	 * @return If we are running an interaction trace.
	 */
	UFUNCTION(BlueprintPure, Category = "Interact|Trace")
	bool IsRunning();
	/**
	 * @brief Interact With Target Actor
	 *
	 * Checks if 'TargetActor' is valid and if so, tries interacting with it.
	 */
	UFUNCTION(Blueprintcallable, Category = "Interact|Trace")
	void InteractWithTargetActor();
};
