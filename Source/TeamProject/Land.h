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

	UPROPERTY(VisibleAnywhere, Category = "Landmass")
		UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditAnywhere, Category = "Landmass")
		UInstancedStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Landmass")
		int32 Size = 250;

	UPROPERTY(EditAnywhere, Category = "Landmass")
		float Scale = 400;

	UPROPERTY(EditAnywhere, Category = "Landmass")
		int WaterLevel = -1800;


	virtual void OnConstruction(const FTransform& Transform) override;

	void CreateMesh();

	UFUNCTION(CallInEditor, Category = "LandMass")
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
