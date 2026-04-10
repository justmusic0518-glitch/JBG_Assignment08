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
	
	int32 CurrentWaveLevelIndex;
	int32 CurrentLevelIndex;
	
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void OnTransitionFinished();
	void ShowTransitionWidget();
};
