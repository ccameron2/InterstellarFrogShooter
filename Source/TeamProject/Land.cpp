// Fill out your copyright notice in the Description page of Project Settings.

#include "Land.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ALand::ALand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Land Mesh"));
	SetRootComponent(ProcMesh);
}

// Called when the game starts or when spawned
void ALand::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALand::OnConstruction(const FTransform& Transform)
{
	if (!MeshCreated)
	{
		CreateMesh();
	}
}

void ALand::CreateMesh()
{
	Vertices.Empty();
	Triangles.Empty();
	Vertices.Init({ 0,0,0 }, Size * Size);

	int indexX = 0;
	for (int i = -Size/2; i < Size/2; i++)
	{
		int indexY = 0;
		for (int j = -Size/2; j < Size / 2; j++)
		{
			FVector newVector = FVector{ i * Scale,j * Scale,0 };
			Vertices[Size * indexX + indexY] = newVector;
			indexY++;
		}
		indexX++;
	}
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Size, Size, false, Triangles);

	float tallestVectorHeight = 0;
	int tallestVector = 0;
	for (int i = 0; i < Vertices.Num(); i++)
	{
		
		auto result = FMath::PerlinNoise2D(0.6 * FVector2D(Vertices[i].X, Vertices[i].Y) / 2000);
		result += FMath::PerlinNoise2D(0.3 * FVector2D(Vertices[i].X, Vertices[i].Y) / 2000);
		result += FMath::PerlinNoise2D(0.2 * FVector2D(Vertices[i].X, Vertices[i].Y) / 2000);
		Vertices[i].Z += result * 1000;
		if (Vertices[i].Z > tallestVectorHeight)
		{
			tallestVector = i;
			tallestVectorHeight = Vertices[i].Z;
		}
	}

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	ProcMesh->ClearMeshSection(0);
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);
	MeshCreated = true;
}

void ALand::MakeNewMesh()
{
	CreateMesh();
}

