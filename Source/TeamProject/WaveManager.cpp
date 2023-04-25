// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveManager.h"

#include "MainPlayerController.h"

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
	ActualWaveTime = StartingWaveTime;
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < EnemyFrogs.Num(); i++)
	{
		if (EnemyFrogs[i] == nullptr || EnemyFrogs[i]->Health <= 0)
		{
 			EnemyFrogs.RemoveAt(i);
			NumAliveFrogs--;
		}
	}
}

void AWaveManager::NewWave()
{

	if (NumAliveFrogs < MaxFrogs)
	{
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::NewWave, ActualWaveTime);
		if (MaxWaves != 0)
		{
			if (WaveNum > MaxWaves)
			{
				GetWorldTimerManager().ClearTimer(WaveTimerHandle);
				GetWorldTimerManager().ClearTimer(WaveDisplayTimerHandle);
				return;
			}
		}

		GetWorldTimerManager().SetTimer(WaveDisplayTimerHandle, this, &AWaveManager::ResetWaveChanged, WaveDisplayTime, false);
		WaveChanged = true;
		NumFrogsToSpawn += WaveNum * 2;

		CurrentDirection = static_cast<WaveDirection>(FMath::RandRange(0, 3));

		for (int i = 0; i < NumFrogsToSpawn; i++)
		{
			FTransform transform;
			auto location = GetNewFrogLocation(CurrentDirection);
			transform.SetTranslation(location);

			EnemyFrogs.Push(GetWorld()->SpawnActor<AAICharacter>(AIClass, transform));
			NumAliveFrogs++;
		}

		WaveNum++;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(WaveDelayHandle, this, &AWaveManager::NewWave, MaxFrogsWaveDelay);
	}
	//GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	
}

void AWaveManager::ClearFrogs()
{
	if (EnemyFrogs.Num() > 0)
	{
		for (auto& frog : EnemyFrogs)
		{
			frog->Destroy();
		}
		EnemyFrogs.Empty();
	}
}

void AWaveManager::Init(int worldSize)
{
	WorldSize = worldSize;
}

void AWaveManager::StartWaves()
{
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::NewWave, ActualWaveTime);
	ActualWaveTime = WaveTime;
}

void AWaveManager::SpawnTitleFrogs()
{
	for (int i = 0; i < NumTitleFrogs; i++)
	{
		float x = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		float y = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		FTransform transform;
		FHitResult Hit;
		FVector Start = FVector{ x,y,5000 };
		FVector End = FVector{ x,y,-5000 };
		ECollisionChannel Channel = ECC_Visibility;
		FCollisionQueryParams Params;
		
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

		
		FVector location = Hit.Location;
		location.Z += 50;
		transform.SetTranslation(location);
		auto newFrog = GetWorld()->SpawnActor<AAICharacter>(AIClass, transform);
		if(newFrog) EnemyFrogs.Push(newFrog);
	}
}

FVector AWaveManager::GetNewFrogLocation(WaveDirection direction)
{
	float x = 0;
	float y = 0;
	switch (direction)
	{
	case North:
		x = FMath::RandRange(WorldSize / 3, WorldSize / 2);
		y = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		break;
	case South:
		x = FMath::RandRange(-WorldSize / 2, -WorldSize / 3);
		y = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		break;
	case East:
		x = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		y = FMath::RandRange(WorldSize / 3, WorldSize / 2);
		break;
	case West:
		x = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		y = FMath::RandRange(-WorldSize / 2, -WorldSize / 3);
		break;
	}

	FHitResult Hit;
	FVector Start = FVector{ x,y,5000 };
	FVector End = FVector{ x,y,-5000 };
	ECollisionChannel Channel = ECC_Visibility;
	FCollisionQueryParams Params;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if(bDebug)
		DrawDebugLine(GetWorld(), Start, Hit.Location, FColor(125, 18, 255), true, -1, 0, 20);

	FVector location = Hit.Location;
	location.Z += 50;

	return location;
}

