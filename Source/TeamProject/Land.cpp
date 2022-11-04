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
		MeshCreated = true;
	}
}

void ALand::CreateMesh()
{

	Vertices.Init({ 0,0,0 }, Size * Size);

	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			FVector newVector = FVector{ i * Scale,j * Scale,0 };
			Vertices[Size * i + j] = newVector;
		}
	}
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Size, Size, false, Triangles);

	for (auto& vertex : Vertices)
	{
		vertex.Z = 1000 * FMath::PerlinNoise2D(FVector2D(vertex.X / 1000, vertex.Y / 1000));
	}

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);
}

