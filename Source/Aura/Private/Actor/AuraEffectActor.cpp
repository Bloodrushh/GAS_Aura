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
		const FGameplayEffectSpecHandle effectSpecHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, effectContextHandle);
		targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
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
}
