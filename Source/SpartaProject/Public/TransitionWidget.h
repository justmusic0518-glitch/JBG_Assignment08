// TransitionWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TransitionWidget.generated.h"

UCLASS()
class JBG_ASSIGNMENT08_API UTransitionWidget : public UUserWidget{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	int32 CurrentWaveLevelIndex = 0;
	int32 CurrentLevelIndex = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void OnTransitionFinished();
	void ShowTransitionWidget();
};
