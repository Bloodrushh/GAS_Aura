// Copyright Rustam Shamaev


#include "UI/HUD/AuraHUD.h"

#include "UI/Widgets/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	widget->AddToViewport();
}
