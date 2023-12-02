// Copyright Rustam Shamaev


#include "Characters/AuraPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuraPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info for the server
	InitAbilityActorInfo();
}

void AAuraPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init Ability Actor Info for the client
	InitAbilityActorInfo();
}

void AAuraPlayerCharacter::InitAbilityActorInfo()
{
	auto const playerState = GetPlayerState<AAuraPlayerState>();
	check(playerState);
	playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
	Cast<UAuraAbilitySystemComponent>(playerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = playerState->GetAbilitySystemComponent();
	AttributeSet = playerState->GetAttributeSet();

	if(AAuraPlayerController* auraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD* auraHUD = Cast<AAuraHUD>(auraPlayerController->GetHUD()))
		{
			auraHUD->InitOverlay(auraPlayerController, playerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
