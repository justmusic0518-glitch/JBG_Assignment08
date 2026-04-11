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

}

void UTransitionWidget::ShowTransitionWidget(){
	// LevelIndex 초기화
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			if (SpartaGameInstance)
			{
				CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
				CurrentWaveLevelIndex = SpartaGameInstance->CurrentWaveLevelIndex;
				UE_LOG(LogTemp, Warning, TEXT("[%p] Level %d, Wave %d GameInstance"), this, CurrentLevelIndex+1,CurrentWaveLevelIndex+1);
			}
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* PC = Cast<ASprataPlayerController>(PlayerController))
		{
			
			PC->ClearAllWidget();
			if (PC->TransitionWidgetClass)
			{
				UUserWidget* TWI = PC->TransitionWidgetInstance;
				TWI->AddToViewport(100);
			
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
				if (UTextBlock* LevelText = Cast<UTextBlock>(TWI->GetWidgetFromName(TEXT("TransitionLevel"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}
				if (UTextBlock* WaveText = Cast<UTextBlock>(TWI->GetWidgetFromName(TEXT("TransitionWave"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), CurrentWaveLevelIndex + 1)));
				}
				UE_LOG(LogTemp, Warning, TEXT("[%p] Level %d, Wave %d"), this, CurrentLevelIndex+1,CurrentWaveLevelIndex+1);
				UFunction* PlayAnimFunc = TWI->FindFunction(FName("PlayTransitionAnim"));
				if (PlayAnimFunc) { TWI->ProcessEvent(PlayAnimFunc, nullptr); }
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
