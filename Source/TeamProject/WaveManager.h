// Fill out your copyright notice in the Description page of Project Settings.

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

	void NewWave();
	void ClearFrogs();
	void Init(int worldSize);
	void StartWaves();
	void SpawnTitleFrogs();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AAICharacter> AIClass;
	
	UPROPERTY(EditAnywhere)
		int MaxWaves = 5;

	TArray<AAICharacter*> EnemyFrogs;

	FTimerHandle WaveDelayHandle;
	FTimerHandle WaveTimerHandle;
	FTimerHandle WaveDisplayTimerHandle;

	UPROPERTY(EditAnywhere)
		float StartingWaveTime = 120;
	
	UPROPERTY(EditAnywhere)
		float WaveTime = 55;

	UPROPERTY()
		float ActualWaveTime;

	UPROPERTY(EditAnywhere)
		float WaveDisplayTime = 2.0f;

	UPROPERTY(EditAnywhere)
		float WaveDelay = 5;

	UPROPERTY(EditAnywhere)
		float MaxFrogsWaveDelay = 30.0f;

	UPROPERTY(BlueprintReadOnly)
		int WaveNum = 0;

	UPROPERTY(EditAnywhere)
		int NumFrogsToSpawn = 5;

	UPROPERTY(EditAnywhere)
		int NumAliveFrogs = 0;

	UPROPERTY(EditAnywhere)
		int MaxFrogs = 60;

	UPROPERTY(EditAnywhere)
		int NumTitleFrogs = 35;

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
