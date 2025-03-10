// Copyright 2025 - Lewis Herbert


#include "Components/InteractionComponent.h"

DEFINE_LOG_CATEGORY(InteractionDebug);

UInteractionComponent::UInteractionComponent()
{
	// We don't need to use tick so lets disable it
	PrimaryComponentTick.bCanEverTick = false;
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bStartOnBeginPlay)
		StartInteractionTrace();
	
}

void UInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopInteractionTrace();
	HandleDebug(EInteractionLogType::Log, "UInteractionComponent has been destroyed");

	Super::EndPlay(EndPlayReason);
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::HandleDebug(EInteractionLogType InLogType, FString Message)
{
	#if !UE_BUILD_SHIPPING
		if (bPrintDebug)
		{
			switch (InLogType)
			{
			case EInteractionLogType::Error:
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
				break;
			case EInteractionLogType::Log:
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, Message);
				break;
			case EInteractionLogType::Warning:
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
				break;
			case EInteractionLogType::Success:
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
				break;
			}
		}

		switch (InLogType)
		{
		case EInteractionLogType::Error:
			UE_LOG(InteractionDebug, Error, TEXT("%s"), *Message);
			break;
		case EInteractionLogType::Log:
			UE_LOG(InteractionDebug, Log, TEXT("%s"), *Message);
			break;
		case EInteractionLogType::Warning:
			UE_LOG(InteractionDebug, Warning, TEXT("%s"), *Message);
			break;
		case EInteractionLogType::Success:
			UE_LOG(InteractionDebug, Display, TEXT("%s"), *Message);
			break;
		}
	#endif
}

bool UInteractionComponent::SetTargetActor(AActor* NewTarget)
{
	if (!IsValid(NewTarget))
	{
		HandleDebug(EInteractionLogType::Error, "UInteractionComponent::SetTargetActor: Target is not valid");
		ClearTargetActor();
		return false;
	}

	if (!NewTarget->Implements<UInteractionInterface>())
	{
		HandleDebug(EInteractionLogType::Error, "UInteractionComponent::SetTargetActor: Target does not implement 'UInteractionInterface'");
		ClearTargetActor();
		return false;
	}

	if (!IsValid(TargetActor))
	{
		if (NewTarget->Implements<UInteractionInterface>())
			IInteractionInterface::Execute_BecomeTarget(NewTarget);

		TargetActor = NewTarget;
		OnTargetActorUpdated.Broadcast(NewTarget);
		HandleDebug(EInteractionLogType::Success, "UInteractionComponent::SetTargetActor: Target has been set");
		return true;
	}
	else
	{
		if (TargetActor != NewTarget)
		{
			if (TargetActor->Implements<UInteractionInterface>())
				IInteractionInterface::Execute_ForgottenAsTarget(TargetActor);

			if (NewTarget->Implements<UInteractionInterface>())
				IInteractionInterface::Execute_BecomeTarget(NewTarget);

			TargetActor = NewTarget;
			OnTargetActorUpdated.Broadcast(NewTarget);
			HandleDebug(EInteractionLogType::Success, "UInteractionComponent::SetTargetActor: Target has been set");
			return true;
		}
		else
		{
			HandleDebug(EInteractionLogType::Warning, "UInteractionComponent::SetTargetActor: Same Target");
			return false;
		}
			
	}
}

bool UInteractionComponent::ClearTargetActor()
{
	if (!IsValid(TargetActor))
		return false;

	if (TargetActor->Implements<UInteractionInterface>())
		IInteractionInterface::Execute_ForgottenAsTarget(TargetActor);

	OnTargetActorForgotten.Broadcast(TargetActor);
	TargetActor = nullptr;
	return true;
}

void UInteractionComponent::StartInteractionTrace()
{
	// Start a repeating timer for tracing
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UInteractionComponent::PerformInteractionTrace, InteractionRate, true);
}

void UInteractionComponent::StopInteractionTrace()
{
	if (!GetWorld()->GetTimerManager().TimerExists(TraceTimerHandle))
		return;

	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}

bool UInteractionComponent::IsRunning()
{
	return GetWorld()->GetTimerManager().IsTimerActive(TraceTimerHandle);
}

void UInteractionComponent::PerformInteractionTrace()
{

	AActor* O = GetOwner();
	if (!O)
		return;

	APlayerController* PC = Cast<APlayerController>(O->GetInstigatorController());
	if (!PC)
		return;

	// Get the player's camera location and direction
	FVector CL;
	FRotator CR;
	PC->GetPlayerViewPoint(CL, CR);
	FVector TE = CL + CR.Vector() * InteractionRange;
	CL = CL + CR.Vector() * InteractionStartOffset;

	// Define trace parameters
	FHitResult HR;
	FCollisionQueryParams TP(FName(TEXT("InteractionTrace")), false, O);

	// Perform the trace
	bool bH = GetWorld()->LineTraceSingleByChannel(HR, CL, TE, ECC_Visibility, TP);

	// Debug draw if enabled
	if (bPrintDebugTrace)
	{
		FColor LC = bH ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), CL, TE, LC, false, 0.1f, 0, 2.0f);
	}

	// Set the target actor if hit
	if (bH && HR.GetActor())
		SetTargetActor(HR.GetActor());
	else
		ClearTargetActor();
}

void UInteractionComponent::InteractWithTargetActor()
{
	if (IsValid(TargetActor) && TargetActor->Implements<UInteractionInterface>())
		IInteractionInterface::Execute_Interact(TargetActor);
}

