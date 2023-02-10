// CCameron
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "Blocker.h"
#include "LandObject.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UENUM()
enum TerrainTypes
{
	Forest,
	Snowy,
	Mossy,
	Piney,
	Desert,
};

UCLASS()
class TEAMPROJECT_API ALand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALand();

	void Init(int seed, int type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		UProceduralMeshComponent* ProcMesh;

	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		TArray<ALandObject*> LandObjects;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ALandObject> LandObjectClass;

	TArray<UInstancedStaticMeshComponent*> StaticMeshes;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TEnumAsByte<TerrainTypes> TerrainType = Forest;

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
		int32 Size = 100;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		uint64 Seed = 69420;
	
	UPROPERTY(EditAnywhere, Category = "ProcGen")
		float Scale = 225;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		int WaterLevel = -2000;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		bool CreateOnConstruction = false;

	UMaterialInterface* WaterMaterial;
	UMaterialInterface* ForestMaterial;
	UMaterialInterface* DesertMaterial;
	UMaterialInterface* MossyMaterial;
	UMaterialInterface* PineyMaterial;
	UMaterialInterface* SnowyMaterial;

	void CreateMesh();
	void ClearMeshInstances();
	void Clear();
	void CalculateNormals();

	int NumTypes = 5;

	UFUNCTION(CallInEditor, Category = "ProcGen")
		void MakeNewMesh();

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FVector2D> UV1s;
	TArray<FColor> VertexColours;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector> WaterVertices;
	TArray<int32> WaterTriangles;
	TArray<FVector> WaterNormals;
	TArray<FColor> WaterColours;
	TArray<FProcMeshTangent> WaterTangents;

	void LoadStaticMeshes();

};
