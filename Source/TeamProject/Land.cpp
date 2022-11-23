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

	//TerrainType = TerrainTypes(FMath::RandRange(0, 4));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset5(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset6(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset7(TEXT("StaticMesh'/Game/LowPolyAssets/WoodLog'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset8(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset9(TEXT("StaticMesh'/Game/LowPolyAssets/BushBerries_1'"));

	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree1 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree2 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree3 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree4 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree5 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Stump Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Log Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Bush Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Berry Bush Static Mesh")));

	ForestStaticMeshes[0]->SetStaticMesh(MeshAsset1.Object);
	ForestStaticMeshes[1]->SetStaticMesh(MeshAsset2.Object);
	ForestStaticMeshes[2]->SetStaticMesh(MeshAsset3.Object);
	ForestStaticMeshes[3]->SetStaticMesh(MeshAsset4.Object);
	ForestStaticMeshes[4]->SetStaticMesh(MeshAsset5.Object);
	ForestStaticMeshes[5]->SetStaticMesh(MeshAsset6.Object);
	ForestStaticMeshes[6]->SetStaticMesh(MeshAsset7.Object);
	ForestStaticMeshes[7]->SetStaticMesh(MeshAsset8.Object);
	ForestStaticMeshes[8]->SetStaticMesh(MeshAsset9.Object);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset11(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset12(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset13(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset14(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset15(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset16(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump_Snow'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset17(TEXT("StaticMesh'/Game/LowPolyAssets/WoodLog_Snow'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset18(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_Snow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset19(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_Snow_2'"));

	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree1 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree2 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree3 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree4 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree5 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Stump Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Log Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Bush Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Other Bush Static Mesh")));
	
	SnowyStaticMeshes[0]->SetStaticMesh(MeshAsset11.Object);
	SnowyStaticMeshes[1]->SetStaticMesh(MeshAsset12.Object);
	SnowyStaticMeshes[2]->SetStaticMesh(MeshAsset13.Object);
	SnowyStaticMeshes[3]->SetStaticMesh(MeshAsset14.Object);
	SnowyStaticMeshes[4]->SetStaticMesh(MeshAsset15.Object);
	SnowyStaticMeshes[5]->SetStaticMesh(MeshAsset16.Object);
	SnowyStaticMeshes[6]->SetStaticMesh(MeshAsset17.Object);
	SnowyStaticMeshes[7]->SetStaticMesh(MeshAsset18.Object);
	SnowyStaticMeshes[8]->SetStaticMesh(MeshAsset19.Object);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset21(TEXT("StaticMesh'/Game/LowPolyAssets/Pine_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset22(TEXT("StaticMesh'/Game/LowPolyAssets/Pine_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset23(TEXT("StaticMesh'/Game/LowPolyAssets/Pine_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset24(TEXT("StaticMesh'/Game/LowPolyAssets/Pine_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset25(TEXT("StaticMesh'/Game/LowPolyAssets/Pine_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset26(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset27(TEXT("StaticMesh'/Game/LowPolyAssets/WoodLog'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset28(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset29(TEXT("StaticMesh'/Game/LowPolyAssets/BushBerries_1'"));

	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree1 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree2 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree3 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree4 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree5 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Stump Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Log Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Bush Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Berry Bush Static Mesh")));

	PineyStaticMeshes[0]->SetStaticMesh(MeshAsset21.Object);
	PineyStaticMeshes[1]->SetStaticMesh(MeshAsset22.Object);
	PineyStaticMeshes[2]->SetStaticMesh(MeshAsset23.Object);
	PineyStaticMeshes[3]->SetStaticMesh(MeshAsset24.Object);
	PineyStaticMeshes[4]->SetStaticMesh(MeshAsset25.Object);
	PineyStaticMeshes[5]->SetStaticMesh(MeshAsset26.Object);
	PineyStaticMeshes[6]->SetStaticMesh(MeshAsset27.Object);
	PineyStaticMeshes[7]->SetStaticMesh(MeshAsset28.Object);
	PineyStaticMeshes[8]->SetStaticMesh(MeshAsset29.Object);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset31(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset32(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset33(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset34(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset35(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset36(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset37(TEXT("StaticMesh'/Game/LowPolyAssets/WoodLog'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset38(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset39(TEXT("StaticMesh'/Game/LowPolyAssets/BushBerries_1'"));

	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree1 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree2 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree3 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree4 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree5 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Stump Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Log Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Bush Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Berry Bush Static Mesh")));
	
	MossyStaticMeshes[0]->SetStaticMesh(MeshAsset31.Object);
	MossyStaticMeshes[1]->SetStaticMesh(MeshAsset32.Object);
	MossyStaticMeshes[2]->SetStaticMesh(MeshAsset33.Object);
	MossyStaticMeshes[3]->SetStaticMesh(MeshAsset34.Object);
	MossyStaticMeshes[4]->SetStaticMesh(MeshAsset35.Object);
	MossyStaticMeshes[5]->SetStaticMesh(MeshAsset36.Object);
	MossyStaticMeshes[6]->SetStaticMesh(MeshAsset37.Object);
	MossyStaticMeshes[7]->SetStaticMesh(MeshAsset38.Object);
	MossyStaticMeshes[8]->SetStaticMesh(MeshAsset39.Object);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset41(TEXT("StaticMesh'/Game/LowPolyAssets/Cactus_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset42(TEXT("StaticMesh'/Game/LowPolyAssets/Cactus_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset43(TEXT("StaticMesh'/Game/LowPolyAssets/Cactus_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset44(TEXT("StaticMesh'/Game/LowPolyAssets/Cactus_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset45(TEXT("StaticMesh'/Game/LowPolyAssets/Cactus_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset46(TEXT("StaticMesh'/Game/LowPolyAssets/CactusFlower_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset47(TEXT("StaticMesh'/Game/LowPolyAssets/CactusFlowers_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset48(TEXT("StaticMesh'/Game/LowPolyAssets/CactusFlowers_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset49(TEXT("StaticMesh'/Game/LowPolyAssets/CactusFlowers_4'"));

	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus1 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus2 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus3 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus4 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus5 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus Flowers 1 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus Flowers 2 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus Flowers 3 Static Mesh")));
	DesertStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cactus Flowers 4 Static Mesh")));
	
	DesertStaticMeshes[0]->SetStaticMesh(MeshAsset41.Object);
	DesertStaticMeshes[1]->SetStaticMesh(MeshAsset42.Object);
	DesertStaticMeshes[2]->SetStaticMesh(MeshAsset43.Object);
	DesertStaticMeshes[3]->SetStaticMesh(MeshAsset44.Object);
	DesertStaticMeshes[4]->SetStaticMesh(MeshAsset45.Object);
	DesertStaticMeshes[5]->SetStaticMesh(MeshAsset46.Object);
	DesertStaticMeshes[6]->SetStaticMesh(MeshAsset47.Object);
	DesertStaticMeshes[7]->SetStaticMesh(MeshAsset48.Object);
	DesertStaticMeshes[8]->SetStaticMesh(MeshAsset49.Object);
	
}

/// Content / LowPolyAssets / CommonTree_1.uasset

// Called when the game starts or when spawned
void ALand::BeginPlay()
{
	Super::BeginPlay();
	if (!MeshCreated)
	{
		CreateMesh();
	}
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
		GeneratedSize != Size || GeneratedSeed != Seed || TerrainType != GeneratedTerrainType)
	{
		MeshCreated = false;
		CreateMesh();
	}
}

void ALand::ClearMeshInstances()
{
	for (auto& mesh : ForestStaticMeshes)
	{
		mesh->ClearInstances();
	}
	for (auto& mesh : SnowyStaticMeshes)
	{
		mesh->ClearInstances();
	}
	for (auto& mesh : PineyStaticMeshes)
	{
		mesh->ClearInstances();
	}
	for (auto& mesh : MossyStaticMeshes)
	{
		mesh->ClearInstances();
	}
	for (auto& mesh : DesertStaticMeshes)
	{
		mesh->ClearInstances();
	}
}

void ALand::CreateMesh()
{
	ClearMeshInstances();
	Vertices.Empty();
	Triangles.Empty();
	Vertices.Init({ 0,0,0 }, Size * Size);

	if (TerrainType == Forest) { StaticMeshes = ForestStaticMeshes; }
	else if(TerrainType == Snowy) { StaticMeshes = SnowyStaticMeshes; }
	else if (TerrainType == Piney) { StaticMeshes = PineyStaticMeshes; }
	else if (TerrainType == Mossy) { StaticMeshes = MossyStaticMeshes; }
	else if (TerrainType == Desert) { StaticMeshes = DesertStaticMeshes; }

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
	noise.SetSeed(Seed);
	float tallestVectorHeight = 0;
	int tallestVector = 0;

	for (int i = 0; i < Vertices.Num(); i++)
	{
		auto input = Vertices[i];

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

	//CalculateNormals();

	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	ProcMesh->ClearMeshSection(0);
	ProcMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColours, Tangents, true);

	/*if (terrainType != Desert)*/
	std::vector<double> coords;

	for (auto vertex : Vertices)
	{
		if (vertex.Z < WaterLevel)
		{
			vertex.Z = WaterLevel;
			WaterVertices.Push(vertex);
			WaterColours.Push(FColor::Blue);\
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
			
			transform.SetScale3D(FVector{ 6,6,6 });

			FQuat Rotation = FVector{ 0,0,0}.ToOrientationQuat();
			transform.SetRotation(Rotation);

			auto staticMesh = StaticMeshes[FMath::RandRange(0, StaticMeshes.Num() - 1)];

			if(staticMesh){ staticMesh->AddInstance(transform); }			
		}
	}

	WaterVertices.Empty(); WaterTriangles.Empty(); WaterColours.Empty();

	GeneratedTerrainType = TerrainType;
	GeneratedScale = Scale;
	GeneratedSize = Size;
	GeneratedWaterLevel = WaterLevel;
	MeshCreated = true;
}

void ALand::MakeNewMesh()
{
	CreateMesh();
}

void ALand::CalculateNormals()
{
	Normals.Init({ 0,0,0 }, Vertices.Num());
	for (int i = 0; i < Triangles.Num(); i+=3)
	{
		auto v1 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i + 1]];
		auto v2 = Vertices[Triangles[i]] - Vertices[Triangles[i + 1]];
		auto n = v1.Cross(v2);
		n.Normalize();
		Normals.Push(n);
	}
}