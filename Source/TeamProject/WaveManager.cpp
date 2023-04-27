// CCameron

#include "WaveManager.h"

#include "MainPlayerController.h"

// Sets default values
AWaveManager::AWaveManager()
{
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

	// Remove dead frogs from EnemyFrogs array
	for (int i = 0; i < EnemyFrogs.Num(); i++)
	{
		if (EnemyFrogs[i] == nullptr || EnemyFrogs[i]->Health <= 0)
		{
 			EnemyFrogs.RemoveAt(i);
			NumAliveFrogs--;
		}
	}
}

// Start a new enemy wave. Sets a timer to call itself again, will delay the next wave if max frogs is hit.
void AWaveManager::NewWave()
{
	if (NumAliveFrogs < MaxFrogs)
	{
		// Set a timer to call this function again
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::NewWave, ActualWaveTime);
		
		// Static win condition (DISABLED)
		if (MaxWaves != 0)
		{
			if (WaveNum > MaxWaves)
			{
				GetWorldTimerManager().ClearTimer(WaveTimerHandle);
				GetWorldTimerManager().ClearTimer(WaveDisplayTimerHandle);
				return;
			}
		}

		// Enable UI popup to tell the player a new wave is starting
		GetWorldTimerManager().SetTimer(WaveDisplayTimerHandle, this, &AWaveManager::ResetWaveChanged, WaveDisplayTime, false);
		WaveChanged = true;

		// Increase the number of frogs to spawn by an increasing amount each wave
		NumFrogsToSpawn += WaveNum * 2;

		// Get side of the world that the enemies should be spawned on
		CurrentDirection = static_cast<WaveDirection>(FMath::RandRange(0, 3));

		// Spawn frogs at a random location on the correct side of the world
		for (int i = 0; i < NumFrogsToSpawn; i++)
		{
			FTransform transform;
			
			// Get a location for current direction
			auto location = GetNewFrogLocation(CurrentDirection);
			transform.SetTranslation(location);

			// Spawn new enemy
			EnemyFrogs.Push(GetWorld()->SpawnActor<AAICharacter>(AIClass, transform));
			NumAliveFrogs++;
		}

		WaveNum++;
	}
	else
	{
		// Delay the next wave until max frogs is no longer reached
		GetWorld()->GetTimerManager().SetTimer(WaveDelayHandle, this, &AWaveManager::NewWave, MaxFrogsWaveDelay);
	}	
}

// Destroy all currently alive frogs
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

// Spawn frogs for the background title screen
void AWaveManager::SpawnTitleFrogs()
{
	for (int i = 0; i < NumTitleFrogs; i++)
	{
		// Get random location in the world
		float x = FMath::RandRange(-WorldSize / 2, WorldSize / 2);
		float y = FMath::RandRange(-WorldSize / 2, WorldSize / 2);

		// Raycast to find the floor
		FTransform transform;
		FHitResult Hit;
		FVector Start = FVector{ x,y,5000 };
		FVector End = FVector{ x,y,-5000 };
		ECollisionChannel Channel = ECC_Visibility;
		FCollisionQueryParams Params;		
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

		// Set location slightly above floor
		FVector location = Hit.Location;
		location.Z += 50;

		// Spawn new frog
		transform.SetTranslation(location);
		auto newFrog = GetWorld()->SpawnActor<AAICharacter>(AIClass, transform);
		if(newFrog) EnemyFrogs.Push(newFrog);
	}
}

// Return a random vector on the specified side of the world.
FVector AWaveManager::GetNewFrogLocation(WaveDirection direction)
{
	float x = 0;
	float y = 0;

	// Set X and Y to correct values per direction
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

	// Raycast to find the floor
	FHitResult Hit;
	FVector Start = FVector{ x,y,5000 };
	FVector End = FVector{ x,y,-5000 };
	ECollisionChannel Channel = ECC_Visibility;
	FCollisionQueryParams Params;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if(bDebug)
		DrawDebugLine(GetWorld(), Start, Hit.Location, FColor(125, 18, 255), true, -1, 0, 20);

	// Increase height slightly from the floor
	FVector location = Hit.Location;
	location.Z += 50;

	return location;
}

