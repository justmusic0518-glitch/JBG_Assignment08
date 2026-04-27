//SpartaGameState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"


UCLASS()
class JBG_ASSIGNMENT08_API ASpartaGameState : public AGameState{
	GENERATED_BODY()
public:
	ASpartaGameState();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 MaxSpawnCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 GoalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	float GoalScorePercent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentWaveLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
	int32 ExplodeMineCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	bool bIsEndingLevel;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	
	FTimerHandle StartTimerHandle;
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void RealStartGame();
	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void OnExplodedMine();
	void EndLevel();
	void UpdateHUD();
};
