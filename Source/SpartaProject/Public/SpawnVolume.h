// SpawnVolume.h
#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"


class UBoxComponent;

UCLASS()
class JBG_ASSIGNMENT08_API ASpawnVolume : public AActor{
	GENERATED_BODY()

public:
	ASpawnVolume();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawing")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawing")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* WaveDataTable;
	
	AActor* SpawnRandomItem(FWaveData* WaveData);
	FItemSpawnRow* GetRandomItem(FWaveData* WaveData) const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass); // 액터의 하위클래스가 아니면 오류가나게
	FVector GetRandomPointInVolume() const;
	FWaveData* GetWaveData(int32 CurrentLevelIndex, int32 CurrentWaveLevelIndex);
};
