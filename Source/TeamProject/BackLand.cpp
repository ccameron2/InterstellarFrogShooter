// Fill out your copyright notice in the Description page of Project Settings.
#include "BackLand.h"
#include "KismetProceduralMeshLibrary.h"
//#include "External/Delaunator.hpp"

// Sets default values
ABackLand::ABackLand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Land Mesh"));
	SetRootComponent(ProcMesh);

	LoadStaticMeshes();
}

// Called when the game starts or when spawned
void ABackLand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABackLand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Sets default values
void ABackLand::Init(int type, FastNoise* noise)
{
	TerrainType = TEnumAsByte<BackTerrainTypes>(type);
	CreateMesh(noise);
}

void ABackLand::Clear()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();

	WaterVertices.Empty();
	WaterTriangles.Empty();
	WaterNormals.Empty();

	ProcMesh->ClearMeshSection(0);
	ProcMesh->ClearMeshSection(1);

	WaterVertices.Empty();
	WaterTriangles.Empty();
	WaterColours.Empty();
}

void ABackLand::CreateMesh(FastNoise* noise)
{
	// Clear any old data
	Clear();

	Vertices.Init({ 0,0,0 }, Size * Size);

	// Create a grid of vertices
	int indexX = 0;
	for (int i = -Size / 2; i < Size / 2; i++)
	{
		int indexY = 0;
		for (int j = -Size / 2; j < Size / 2; j++)
		{
			FVector newVector = FVector{ i * Scale, j * Scale,0 };
			Vertices[Size * indexX + indexY] = newVector;
			indexY++;
		}
		indexX++;
	}

	// Generate triangles for grid of vertices
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Size, Size, false, Triangles);

	// Store the tallest vector index and height
	float tallestVectorHeight = 0;
	int tallestVector = 0;

	// For each vertex, get 2 different noise values and apply them to vertex hight at different scales.
	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Get input vector from vertex list and sample noise at different levels
		auto input = Vertices[i] + GetActorLocation();
		auto result1 = noise->GetNoise(input.X / 1000, input.Y / 1000);
		Vertices[i].Z += result1 * 5000;
		auto result2 = noise->GetNoise(input.X / 120, input.Y / 120);
		Vertices[i].Z += result2 * 2000;

		// Find the tallest vector and store in variables
		if (Vertices[i].Z > tallestVectorHeight)
		{
			tallestVector = i;
			tallestVectorHeight = Vertices[i].Z;
		}
	}

	// Faster normals
	CalculateNormals(Vertices, Triangles, Normals);

	// Create mesh section
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);

	// Set the material of the mesh depending on type
	switch (TerrainType)
	{
	case BForest:
		ProcMesh->SetMaterial(0, ForestMaterial);
		break;
	case BMossy:
		ProcMesh->SetMaterial(0, MossyMaterial);
		break;
	case BPiney:
		ProcMesh->SetMaterial(0, PineyMaterial);
		break;
	case BDesert:
		ProcMesh->SetMaterial(0, DesertMaterial);
		break;
	case BSnowy:
		ProcMesh->SetMaterial(0, SnowyMaterial);
		break;
	default:
		ProcMesh->SetMaterial(0, ForestMaterial);
		break;
	}
	/*if (terrainType != Desert)*/

	// Store list of coordinates for triangulation
	std::vector<double> coords;

	float lastX = 0;
	float lastY = 0;
	bool inARowX = true;
	bool inARowY = true;

	for (auto vertex : Vertices)
	{
		if (vertex.Z < WaterLevel)
		{
			// Set new vertex to water level and add to array
			vertex.Z = WaterLevel;
			WaterVertices.Push(vertex);
			WaterColours.Push(FColor::Blue);

			// Push vertex into coordinates list
			coords.push_back(vertex.X);
			coords.push_back(vertex.Y);

			// Cant triangulate if all triangles are lined up
			if (lastX == 0 || lastY == 0)
			{
				lastX = vertex.X;
				lastY = vertex.Y;
				continue;
			}
			if (vertex.X != lastX) { inARowX = false; }
			if (vertex.Y != lastY) { inARowY = false; }
			lastX = vertex.X;
			lastY = vertex.Y;
		}
	}

	//if (coords.size() > 6 || coords.size() % 3 == 0)
	//{
	//	if (!inARowX && !inARowY)
	//	{
	//		// Use external library to triangulate
	//		delaunator::Delaunator d(coords);

	//		// Push resultant triangles into triangles list
	//		for (auto& triangle : d.triangles)
	//		{
	//			WaterTriangles.Push(triangle);
	//		}

	//		for (auto& vertex : WaterVertices)
	//		{
	//			vertex.Z += 50 * FMath::PerlinNoise2D(FVector2D{ vertex.X,vertex.Y } / 100);
	//		}

	//		// Calculate Normals
	//		CalculateNormals(WaterVertices, WaterTriangles, WaterNormals);

	//		// Create mesh and set material to water
	//		ProcMesh->CreateMeshSection(1, WaterVertices, WaterTriangles, WaterNormals, UVs, WaterColours, WaterTangents, false);
	//		ProcMesh->SetMaterial(1, WaterMaterial);
	//	}
	//}
}

void ABackLand::CalculateNormals(TArray<FVector> vertices, TArray<int32> triangles, TArray<FVector>& normals)
{
	normals.Init({ 0,0,0 }, vertices.Num());

	for (int i = 0; i < triangles.Num() - 3; i += 3)
	{
		auto a = vertices[triangles[i]];
		auto b = vertices[triangles[i + 1]];
		auto c = vertices[triangles[i + 2]];

		auto v1 = a - b;
		auto v2 = c - b;
		auto n = v1 ^ v2;
		n.Normalize();

		normals[triangles[i]] += n;
		normals[triangles[i + 1]] += n;
		normals[triangles[i + 2]] += n;
	}

	for (auto& normal : normals)
	{
		normal.Normalize();
	}
}

void ABackLand::LoadStaticMeshes()
{
	// Load material and pull from object into material interface
	ConstructorHelpers::FObjectFinder<UMaterial> waterMaterial(TEXT("M_Water_Master'/Game/Materials/M_Water_Master.M_Water_Master'"));
	WaterMaterial = waterMaterial.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> forestMaterial(TEXT("M_Terrain_Forest'/Game/Materials/M_Terrain_Forest.M_Terrain_Forest'"));
	ForestMaterial = forestMaterial.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> desertMaterial(TEXT("M_Terrain_Desert'/Game/Materials/M_Terrain_Desert.M_Terrain_Desert'"));
	DesertMaterial = desertMaterial.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> mossyMaterial(TEXT("M_Terrain_Mossy'/Game/Materials/M_Terrain_Mossy.M_Terrain_Mossy'"));
	MossyMaterial = mossyMaterial.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> pineyMaterial(TEXT("M_Terrain_Piney'/Game/Materials/M_Terrain_Piney.M_Terrain_Piney'"));
	PineyMaterial = pineyMaterial.Object;

	ConstructorHelpers::FObjectFinder<UMaterialInstance> snowyMaterial(TEXT("M_Terrain_Snowy'/Game/Materials/M_Terrain_Snowy.M_Terrain_Snowy'"));
	SnowyMaterial = snowyMaterial.Object;
}