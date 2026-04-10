// TransitionWidget.cpp
#include "TransitionWidget.h"
#include "SpartaGameInstance.h"
#include "SprataPlayerController.h"
#include "Components/TextBlock.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"

void UTransitionWidget::NativeConstruct(){
	Super::NativeConstruct();
	CurrentLevelIndex = 0;
	CurrentWaveLevelIndex = 0;
}

void UTransitionWidget::ShowTransitionWidget(){
	// LevelIndex 초기화
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			if (SpartaGameInstance) { CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex; }
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* PC = Cast<ASprataPlayerController>(PlayerController))
		{
			PC->ClearAllWidget();
			if (PC->TransitionWidgetClass)
			{
				this->AddToViewport(100);
				if (this)
				{
					// TransitionWidgetInstance->SetVisibility(ESlateVisibility::Visible);

					PC->bShowMouseCursor = true;
					PC->SetInputMode(FInputModeUIOnly());
				}
				if (UTextBlock* LevelText = Cast
					<UTextBlock>(this->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}
				UFunction* PlayAnimFunc = this->FindFunction(FName("PlayTransitionAnim"));
				if (PlayAnimFunc) { this->ProcessEvent(PlayAnimFunc, nullptr); }
			}
		}
	}
}

void UTransitionWidget::OnTransitionFinished(){
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		if (SpartaGameState->LevelMapNames.IsValidIndex(CurrentLevelIndex))
		{
			UGameplayStatics::OpenLevel(GetWorld(), SpartaGameState->LevelMapNames[CurrentLevelIndex]);
		}
	}
}
