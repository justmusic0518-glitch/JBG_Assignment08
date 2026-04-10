// SpartaGameState.cpp
#include "SpartaGameState.h"
#include "SprataPlayerController.h"
#include "CoinItem.h"
#include "SpawnVolume.h"
#include "SpartaGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"



ASpartaGameState::ASpartaGameState(){
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	CurrentWaveLevelIndex = 0;
	ExplodeMineCount = 0;
	GoalScorePercent = 0.6f;
	GoalScore = 0;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay(){
	Super::BeginPlay();
	StartLevel();
		
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
		);
}

void ASpartaGameState::RealStartGame(){
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}


int32 ASpartaGameState::GetScore() const{ return Score; }

void ASpartaGameState::AddScore(int32 Amount){
	Score += Amount;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel(){
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* SpartaPlayerController = Cast<ASprataPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
			CurrentWaveLevelIndex = SpartaGameInstance->CurrentWaveLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	Score = 0;
	GoalScore = 0;
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes); 
	
	// int32 MaxSpawnCount = UGameplayStatics::GetData
	
	for (int32 i = 0; i < MaxSpawnCount; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					++SpawnedCoinCount;
					if (ACoinItem* CoinItem = Cast<ACoinItem>(SpawnedActor))
					{
						GoalScore += CoinItem->GetPointValue();
					}
				}
			}
		}
	}
	
	GoalScore *= GoalScorePercent;
	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false 
	);

}

void ASpartaGameState::OnLevelTimeUp(){
	EndLevel();
}

void ASpartaGameState::OnCoinCollected(){
	++CollectedCoinCount;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d, Score : %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount,
		Score,
		GoalScore)

	if (Score >= GoalScore)
	{
		EndLevel();
	}
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::OnExplodedMine(){
	int32 GoalExplodeMineCount = 3;
	
	++ExplodeMineCount;
	UE_LOG(LogTemp, Warning, TEXT("Mine Exploded: %d / %d"),
		ExplodeMineCount,
		GoalExplodeMineCount
	)
	/*
	if (GoalExplodeMineCount <= ExplodeMineCount)
	{
		EndLevel();
	}
	*/
}

void ASpartaGameState::EndLevel(){
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			AddScore(Score);
			++CurrentWaveLevelIndex;
			SpartaGameInstance->CurrentWaveLevelIndex = CurrentWaveLevelIndex;
		}
	}
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* SpartaPlayerController = Cast<ASprataPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowTransition();
		}
	}

	if (CurrentWaveLevelIndex >= MaxLevels)
	{
		++CurrentLevelIndex;
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
			if (SpartaGameInstance)
			{
				SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
				SpartaGameInstance->CurrentWaveLevelIndex = 0;
			}
		}
	}
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}
	
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
	
	
}

void ASpartaGameState::OnGameOver(){
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* SpartaPlayerController = Cast<ASprataPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::UpdateHUD(){
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* SpartaPlayerController = Cast<ASprataPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);//타이머가 몇초 남았는지
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName((TEXT("Time")))))
				{
					UE_LOG(LogTemp, Warning, TEXT("Remaining Time: %f"), RemainingTime);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName((TEXT("Score")))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName((TEXT("Level")))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex +1)));
				}
			}
		}
	}
}
