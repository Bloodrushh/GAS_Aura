// Copyright Rustam Shamaev


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	check(GameplayEffectClass);
	
	if(const auto targetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{		
		FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
		effectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, effectContextHandle);
		const FActiveGameplayEffectHandle activeGameplayEffectHandle = targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());

		if(effectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite && InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveEndOverlap)
		{
			ActiveInfiniteEffectHandles.Add(activeGameplayEffectHandle, targetASC);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if(InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if(DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if(InfiniteGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstantGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if(DurationGameplayEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if(InfiniteGameplayEffectRemovalPolicy == EEffectRemovalPolicy::RemoveEndOverlap)
	{
		UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(targetASC))
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> handlesToRemove;

		for(auto handlePair : ActiveInfiniteEffectHandles)
		{
			if(handlePair.Value == targetASC)
			{
				targetASC->RemoveActiveGameplayEffect(handlePair.Key, 1);
				handlesToRemove.Add(handlePair.Key);
			}
		}

		for(auto& handle : handlesToRemove)
		{
			ActiveInfiniteEffectHandles.FindAndRemoveChecked(handle);
		}
	}
}
