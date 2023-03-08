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

	FTimerHandle WaveTimerHandle;
	float WaveTime = 25;
	float WaveDelay = 5;
	int WaveNum = 0;
	int NumFrogs = 5;
	int NumTitleFrogs = 25;
	int WorldSize = 0;
	WaveDirection CurrentDirection;
private:

	FVector GetNewFrogLocation(WaveDirection direction);
};
