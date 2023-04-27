// CCameron

#pragma once

#include "AICharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

UCLASS()
class TEAMPROJECT_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

	enum WaveDirection
	{
		North,
		South,
		East,
		West
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn a new wave
	void NewWave();

	// Clear enemy frogs
	void ClearFrogs();

	// Set worldsize
	void Init(int worldSize);

	// Start wave countdown
	void StartWaves();

	// Spawn background title frogs
	void SpawnTitleFrogs();

	// Class of enemy to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<AAICharacter> AIClass;
	
	// Max number of waves in static wave system
	UPROPERTY(EditAnywhere)
		int MaxWaves = 5;

	// Array of enemy frogs
	TArray<AAICharacter*> EnemyFrogs;

	// Wave timers
	FTimerHandle WaveDelayHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle WaveDisplayTimerHandle;

	// Time before first wave starts
	UPROPERTY(EditAnywhere)
		float StartingWaveTime = 30;
	
	// Time between waves
	UPROPERTY(EditAnywhere)
		float WaveTime = 55;

	UPROPERTY()
		float ActualWaveTime;

	// Length of time to display UI on screen
	UPROPERTY(EditAnywhere)
		float WaveDisplayTime = 2.0f;

	UPROPERTY(EditAnywhere)
		float WaveDelay = 5;

	// Time to delay next wave when max frogs is hit
	UPROPERTY(EditAnywhere)
		float MaxFrogsWaveDelay = 30.0f;

	UPROPERTY(BlueprintReadOnly)
		int WaveNum = 0;

	// Number of frogs to spawn per wave
	UPROPERTY(EditAnywhere)
		int NumFrogsToSpawn = 5;

	// Current number of frogs alive
	UPROPERTY(EditAnywhere)
		int NumAliveFrogs = 0;

	// Max number of frogs allowed in world
	UPROPERTY(EditAnywhere)
		int MaxFrogs = 100;

	// Number of frogs to spawn on the title screen
	UPROPERTY(EditAnywhere)
		int NumTitleFrogs = 35;

	// Size of the world
	UPROPERTY(EditAnywhere)
		int WorldSize = 0;

	UPROPERTY(BlueprintReadOnly)
		bool WaveChanged = false;

	UPROPERTY(EditAnywhere)
		bool bDebug = false;
	
	WaveDirection CurrentDirection;
private:

	UFUNCTION()
		void ResetWaveChanged() { WaveChanged = false; }

	FVector GetNewFrogLocation(WaveDirection direction);
};
