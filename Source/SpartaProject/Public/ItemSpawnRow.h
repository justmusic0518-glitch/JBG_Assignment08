// ItemSpawnRow.h 
#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(Blueprintable)
struct FItemSpawnRow : public FTableRowBase{
	GENERATED_BODY()
	
public:
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};

USTRUCT(Blueprintable)
struct FWaveData : public FTableRowBase{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSpawnCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSpawnRow> WaveItems;
};