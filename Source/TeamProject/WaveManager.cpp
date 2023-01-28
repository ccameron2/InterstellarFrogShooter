// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveManager.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	NewWave();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveManager::NewWave()
{
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::NewWave, WaveTime, false, WaveDelay);

	NumFrogs += WaveNum * 2;

	for (int i = 0; i < NumFrogs; i++)
	{
		float x = FMath::RandRange(-(100 * 225) / 2, (100 * 225) / 2);
		float y = FMath::RandRange(-(100 * 225) / 2, (100 * 225) / 2);
		FTransform Transform;
		FVector Location = { x,y,3000};
		Transform.SetTranslation(Location);

		AICharacters.Push(GetWorld()->SpawnActor<AAICharacter>(AIClass, Transform));
	}
	
	WaveNum++;
}

void AWaveManager::ClearFrogs()
{
	for (auto& frog : AICharacters)
	{
		frog->Destroy();
	}
}

