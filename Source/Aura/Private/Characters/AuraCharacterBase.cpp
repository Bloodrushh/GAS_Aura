// Copyright Rustam Shamaev


#include "Characters/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}