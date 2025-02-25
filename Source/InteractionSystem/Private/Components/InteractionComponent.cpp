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


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::Deactivate()
{
	Super::Deactivate();

	HandleDebug(EInteractionLogType::Log, "UInteractionComponent has been deactivated");
	StopInteractionTrace();
	ClearTargetActor();
}

void UInteractionComponent::OnUnregister()
{
	Super::OnUnregister();

	HandleDebug(EInteractionLogType::Log, "UInteractionComponent has been unregistered");
	StopInteractionTrace();
	ClearTargetActor();
}

void UInteractionComponent::HandleDebug(EInteractionLogType InLogType, FString Message)
{
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
		IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetActor);

		if (InteractionInterface)
			InteractionInterface->ForgottenAsTarget();

		TargetActor = NewTarget;
		InteractionInterface = Cast<IInteractionInterface>(TargetActor);

		if (InteractionInterface)
			InteractionInterface->BecomeTarget();

		OnTargetActorUpdated.Broadcast(NewTarget);
		HandleDebug(EInteractionLogType::Success, "UInteractionComponent::SetTargetActor: Target has been set");
		return true;
	}
	else
	{
		if (TargetActor != NewTarget)
		{
			IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetActor);

			if (InteractionInterface)
				InteractionInterface->ForgottenAsTarget();

			TargetActor = NewTarget;
			InteractionInterface = Cast<IInteractionInterface>(TargetActor);

			if (InteractionInterface)
				InteractionInterface->BecomeTarget();

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

	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetActor);

	if (InteractionInterface)
		InteractionInterface->ForgottenAsTarget();

	OnTargetActorForgotten.Broadcast(TargetActor);
	TargetActor = NULL;
	return true;
}

void UInteractionComponent::StartInteractionTrace()
{
	// Start a repeating timer for tracing
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UInteractionComponent::PerformInteractionTrace, InteractionRate, true);
}

void UInteractionComponent::StopInteractionTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}

bool UInteractionComponent::IsRunning()
{
	return GetWorld()->GetTimerManager().IsTimerActive(TraceTimerHandle);
}

void UInteractionComponent::PerformInteractionTrace()
{

	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
	if (!PlayerController)
		return;

	// Get the player's camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * InteractionRange;
	CameraLocation = CameraLocation + CameraRotation.Vector() * InteractionStartOffset;

	// Define trace parameters
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("InteractionTrace")), false, Owner);

	// Perform the trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, TraceParams);

	// Debug draw if enabled
	if (bPrintDebugTrace)
	{
		FColor LineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, LineColor, false, 0.1f, 0, 2.0f);
	}

	// Set the target actor if hit
	if (bHit && HitResult.GetActor())
	{
		SetTargetActor(HitResult.GetActor());
	}
	else
	{
		ClearTargetActor();
	}
}

void UInteractionComponent::InteractWithTargetActor()
{
	if (!IsValid(TargetActor))
		return;

	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetActor);

	if (InteractionInterface)
		InteractionInterface->Interact();
}

