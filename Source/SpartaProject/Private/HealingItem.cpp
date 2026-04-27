// Fill out your copyright notice in the Description page of Project Settings.
#include "HealingItem.h"
#include "SprataCharacter.h"


AHealingItem::AHealingItem(){
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASprataCharacter* PlayerCharacter = Cast<ASprataCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		
		DestroyItem();
	}
}


