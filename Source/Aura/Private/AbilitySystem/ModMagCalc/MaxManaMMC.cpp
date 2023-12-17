// Copyright Rustam Shamaev


#include "AbilitySystem/ModMagCalc/MaxManaMMC.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMaxManaMMC::UMaxManaMMC()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMaxManaMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParameters;
	evaluationParameters.SourceTags = sourceTags;
	evaluationParameters.TargetTags = targetTags;

	float intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, evaluationParameters, intelligence);

	intelligence = FMath::Max<float>(intelligence, 0.0f);

	auto const combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();

	return 50.f + 2.5f * intelligence + 15.f * playerLevel;
}
