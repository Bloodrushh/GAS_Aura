// Copyright Rustam Shamaev

#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemyCharacter.generated.h"

UCLASS()
class AURA_API AAuraEnemyCharacter : public AAuraCharacterBase, public  IEnemyInterface
{
public:
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

private:
	GENERATED_BODY()
};
