// Copyright Rustam Shamaev

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraInputContext);

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(subsystem)
	{
		subsystem->AddMappingContext(AuraInputContext, 0);
	}	

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(inputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d inputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator rotation = GetControlRotation();
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f);

	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* controlledPAwn = GetPawn<APawn>())
	{
		controlledPAwn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		controlledPAwn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult cursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	if(!cursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = 	Cast<IEnemyInterface>(cursorHit.GetActor());
	/**
	 * Line trace from cursor. There are several scenarios.
	 * A. LastActor is null && CurrentActor is null
	 *		- do nothing
	 *	B. LastActor is null && CurrentActor is valid
	 *		- Hightlight CurrentActor
	 *	C. LastActor is valid && CurrentActor is null
	 *		- UnHighligh LastActor
	 *	D. Both actors are valid, but LastActor != CurrentActor
	 *		- Unhighlight LastActor and Hightlight CurrentActor
	 *	E. Both actors are valid and are te same actor
	 *		- do nothing
	 */

	if(LastActor == nullptr)
	{
		if(CurrentActor != nullptr)
		{
			//Case B
			CurrentActor->HighlightActor();
		}
		else
		{
			// Case A. both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if(CurrentActor == nullptr)
		{
			// Case C.
			LastActor->UnHighlightActor();
		}
		else // both are valid
		{
			if(LastActor != CurrentActor)
			{
				// Case D.
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				//Case E - do nothing
			}
		}
	}
}
