// Copyright Rustam Shamaev

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "AuraPlayerCharacter.generated.h"

UCLASS()
class AURA_API AAuraPlayerCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Combat interface */

	virtual int32 GetPlayerLevel() const override;

	/* End Combat interface */
	
protected:
	virtual void InitAbilityActorInfo() override;	
};