// Copyright Rustam Shamaev


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// TODO: Change this to apply GameplayEffect. For now using const_cast as a hack!
	if(auto abilityActor = Cast<IAbilitySystemInterface>(OtherActor))
	{
		if(auto abilitySystemComponent = abilityActor->GetAbilitySystemComponent())
		{
			auto const auraAttributeSet = Cast<UAuraAttributeSet>(abilitySystemComponent->GetAttributeSet(UAuraAttributeSet::StaticClass()));
			
			UAuraAttributeSet* mutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(auraAttributeSet);
			mutableAuraAttributeSet->SetHealth(auraAttributeSet->GetHealth() + 25.f);
			Destroy();
		}
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}
