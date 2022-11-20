// Fill out your copyright notice in the Description page of Project Settings.

#include "Land.h"
#include "KismetProceduralMeshLibrary.h"
#include "External/FastNoise.h"
#include "External/Delaunator.hpp"
// Sets default values
ALand::ALand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Land Mesh"));
	SetRootComponent(ProcMesh);

	StaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
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

	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);

	float tallestVectorHeight = 0;
	int tallestVector = 0;
	for (int i = 0; i < Vertices.Num(); i++)
	{
		auto result = noise.GetNoise(Vertices[i].X / 120, Vertices[i].Y / 120);
		auto largerResult = noise.GetNoise(Vertices[i].X / 300, Vertices[i].Y / 300);
		Vertices[i].Z += result * 2000;
		Vertices[i].Z += largerResult * 5000;
		if (Vertices[i].Z > tallestVectorHeight)
		{
			tallestVector = i;
			tallestVectorHeight = Vertices[i].Z;
		}
	}

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	ProcMesh->ClearMeshSection(0);
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);

	std::vector<double> coords;

	for (auto vertex : Vertices)
	{
		if (vertex.Z < WaterLevel)
		{
			vertex.Z = WaterLevel;
			WaterVertices.Push(vertex);
			WaterColours.Push(FColor::Blue);
			coords.push_back(vertex.X);
			coords.push_back(vertex.Y);
		}
	}

	// Use external library to triangulate
	delaunator::Delaunator d(coords);

	// Push resultant triangles into triangles list
	for (auto& triangle : d.triangles)
	{
		WaterTriangles.Push(triangle);
	}
	ProcMesh->ClearMeshSection(1);
	ProcMesh->CreateMeshSection(1, WaterVertices, WaterTriangles, Normals, UVs, WaterColours, Tangents, false);

	//for (auto& vertex : Vertices)
	//{
	//	if (vertex.X > WaterLevel)
	//	{
	//		FTransform transform;
	//		transform.SetLocation(vertex);
	//		
	//		FQuat Rotation = FVector{ 0,0,float(FMath::Rand())}.ToOrientationQuat();
	//		transform.SetRotation(Rotation);

	//		if(StaticMesh){ StaticMesh->AddInstance(transform); }			
	//	}
	//}

	WaterVertices.Empty(); WaterTriangles.Empty(); WaterColours.Empty();

	MeshCreated = true;
}

void ALand::MakeNewMesh()
{
	CreateMesh();
}

