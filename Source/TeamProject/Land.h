// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UCLASS()
class TEAMPROJECT_API ALand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TArray<UInstancedStaticMeshComponent*> StaticMesh;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		int32 Size = 250;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		uint64 Seed = 69420;
	
	UPROPERTY(EditAnywhere, Category = "ProcGen")
		float Scale = 400;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		int WaterLevel = -2000;


	int GeneratedSize = 0;
	int GeneratedScale = 0;
	int GeneratedWaterLevel = 0;
	uint64 GeneratedSeed = 0;

	virtual void OnConstruction(const FTransform& Transform) override;

	void CreateMesh();

	UFUNCTION(CallInEditor, Category = "ProcGen")
		void MakeNewMesh();

	bool MeshCreated = false;

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FVector2D> UV1s;
	TArray<FColor> VertexColours;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector> WaterVertices;
	TArray<int32> WaterTriangles;
	TArray<FColor> WaterColours;

};
