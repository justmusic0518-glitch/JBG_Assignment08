
#include "MineItem.h"

#include "SpartaGameState.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AMineItem::AMineItem(){
	ExplosionDelay = 2.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
		
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator){
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AMineItem::Explode,
		ExplosionDelay,
		false // 반복체크
		);
}

void AMineItem::Explode(){
		
	TArray<AActor*> OverlappingActors; // 왜 만드는걸까
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	if (UWorld* World = GetWorld())
	{
		if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
		{
			GameState->OnExplodedMine();
		}
	}
	
	DestroyItem();
}


