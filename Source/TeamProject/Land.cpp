// Fill out your copyright notice in the Description page of Project Settings.

#include "Land.h"
#include "KismetProceduralMeshLibrary.h"
#include "External/FastNoise.h"
#include "External/Delaunator.hpp"
#include "UObject/ConstructorHelpers.h" 

// Sets default values
ALand::ALand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Land Mesh"));
	SetRootComponent(ProcMesh);


	// This will be replaced and read from file
	auto MeshAsset1 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_1'"));
	auto MeshAsset2 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_2'"));
	auto MeshAsset3 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_3'"));

	StaticMesh.Init(nullptr, 3);

	StaticMesh[0] = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree1 Static Mesh"));
	StaticMesh[1] = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree2 Static Mesh"));
	StaticMesh[2] = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree3 Static Mesh"));

	StaticMesh[0]->SetStaticMesh(MeshAsset1.Object);
	StaticMesh[1]->SetStaticMesh(MeshAsset2.Object);
	StaticMesh[2]->SetStaticMesh(MeshAsset3.Object);
	////////////////////////////////////////////
}

/// Content / LowPolyAssets / CommonTree_1.uasset

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
	
	// Update mesh on editor parameter changes
	if (GeneratedWaterLevel != WaterLevel || GeneratedScale != Scale ||
		GeneratedSize != Size || GeneratedSeed != Seed)
	{
		MeshCreated = false;
		CreateMesh();
	}
}

void ALand::CreateMesh()
{
	for (auto& mesh : StaticMesh)
	{
		mesh->ClearInstances();
	}
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
		auto input = Vertices[i] + Seed;

		auto result1 = noise.GetNoise(input.X / 1000, input.Y / 1000);
		Vertices[i].Z += result1 * 5000;
		auto result2 = noise.GetNoise(input.X / 120, input.Y / 120);
		Vertices[i].Z += result2 * 2000;
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

	if (coords.size() > 0)
	{
		// Use external library to triangulate
		delaunator::Delaunator d(coords);

		// Push resultant triangles into triangles list
		for (auto& triangle : d.triangles)
		{
			WaterTriangles.Push(triangle);
		}
		ProcMesh->ClearMeshSection(1);
		ProcMesh->CreateMeshSection(1, WaterVertices, WaterTriangles, Normals, UVs, WaterColours, Tangents, false);
	}
	
	for (auto& vertex : Vertices)
	{
		if (vertex.Z > WaterLevel && FMath::RandRange(0,100) > 99)
		{
			FTransform transform;
			transform.SetLocation(vertex);
			
			transform.SetScale3D(FVector{ 8,8,8 });

			FQuat Rotation = FVector{ 0,0,0}.ToOrientationQuat();
			transform.SetRotation(Rotation);

			auto staticMesh = StaticMesh[FMath::RandRange(0, StaticMesh.Num() - 1)];

			if(staticMesh){ staticMesh->AddInstance(transform); }			
		}
	}

	WaterVertices.Empty(); WaterTriangles.Empty(); WaterColours.Empty();


	GeneratedScale = Scale;
	GeneratedSize = Size;
	GeneratedWaterLevel = WaterLevel;
	MeshCreated = true;
}

void ALand::MakeNewMesh()
{
	CreateMesh();
}

