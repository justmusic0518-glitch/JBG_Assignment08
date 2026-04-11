// SpawnVolume.cpp
#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume(){
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
	
	ItemDataTable = nullptr;
	WaveDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem(FWaveData* WaveData){
	if (FItemSpawnRow* SelectedRow = GetRandomItem(WaveData))
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(FWaveData* WaveData) const{
	if (!WaveData) return nullptr;
	
	const TArray<FItemSpawnRow>& AllRows = WaveData->WaveItems;
	
	if (AllRows.IsEmpty()) return nullptr;
	
	float TotalChance = 0.0f;
	for (const FItemSpawnRow& Row : AllRows)
	{
		TotalChance += Row.SpawnChance;
	}
	
	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;
	
	for (FItemSpawnRow& Row : WaveData->WaveItems)
	{
		AccumulateChance += Row.SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return &Row;
		}
	}
	return nullptr;
}

	
FVector ASpawnVolume::GetRandomPointInVolume() const{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent(); // 반지름 값
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass){
	if (!ItemClass) return nullptr;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}

FWaveData* ASpawnVolume::GetWaveData(int32 CurrentLevelIndex, int32 CurrentWaveLevelIndex){
	if (!WaveDataTable) return nullptr;
	FString String = FString::Printf(TEXT("Level%d_Wave%d"), CurrentLevelIndex+1, CurrentWaveLevelIndex+1);
	FName Name = FName(*String);
	FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(Name, TEXT("WaveDataContext"));
	return WaveData;
}
