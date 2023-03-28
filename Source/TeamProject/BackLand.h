// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "External/FastNoise.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackLand.generated.h"

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
class TEAMPROJECT_API ABackLand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackLand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Init(int type, FastNoise* noise);
	virtual void Clear();
	virtual void CreateMesh(FastNoise* noise);
	virtual void LoadStaticMeshes();
	virtual void CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles, TArray<FVector>& normals);

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TEnumAsByte<TerrainTypes> TerrainType = Forest;

	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		int32 Size = 100;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		float Scale = 225;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		uint64 Seed = 69420;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		int WaterLevel = -1000;

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

	UMaterialInterface* WaterMaterial;
	UMaterialInterface* ForestMaterial;
	UMaterialInterface* DesertMaterial;
	UMaterialInterface* MossyMaterial;
	UMaterialInterface* PineyMaterial;
	UMaterialInterface* SnowyMaterial;
};
