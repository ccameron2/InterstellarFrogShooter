// CCameron
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "Blocker.h"
#include "LandObject.h"
#include "External/FastNoise.h"


#include "BackLand.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UCLASS()
class TEAMPROJECT_API ALand : public ABackLand
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALand();

	void Init(int type, FastNoise* noise) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		TArray<ALandObject*> LandObjects;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ALandObject> LandObjectClass;

	TArray<UInstancedStaticMeshComponent*> StaticMeshes;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TSubclassOf<ABlocker> BlockerClass;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TArray<ABlocker*> EdgeBlockers;

	TArray<UInstancedStaticMeshComponent*> ForestStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> SnowyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> MossyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> PineyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> DesertStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> FoliageStaticMeshes;


	UPROPERTY(EditAnywhere, Category = "ProcGen")
		bool CreateOnConstruction = false;

	void PlaceObjects(FastNoise* noise);
	void CreateMesh(FastNoise* noise) override;
	void ClearMeshInstances();
	void Clear() override;

	int NumTypes = 5;

	UFUNCTION(CallInEditor, Category = "ProcGen")
		void MakeNewMesh();

	void LoadStaticMeshes() override;

};
