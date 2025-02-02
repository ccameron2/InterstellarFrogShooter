// CCameron
#include "Land.h"
#include "KismetProceduralMeshLibrary.h"
#include "UObject/ConstructorHelpers.h" 

ALand::ALand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadStaticMeshes();
}

// Sets default values
void ALand::Init(int type, FastNoise* noise)
{
	TerrainType = TEnumAsByte<TerrainTypes>(type);
	
	CreateMesh(noise);
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

// Clear land objects
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
	for (auto& mesh : FoliageStaticMeshes)
	{
		mesh->ClearInstances();
	}
}

// Clear geometry and blockers
void ALand::Clear()
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

	ClearMeshInstances();

	if (EdgeBlockers.Num() > 0)
	{
		for (auto& blocker : EdgeBlockers)
		{
			blocker->Destroy();
		}
		EdgeBlockers.Empty();
	}

	if (LandObjects.Num() > 0)
	{
		for (auto& object : LandObjects)
		{
			object->Destroy();
		}
		LandObjects.Empty();
	}
}

// Place obstacles in the world	
void ALand::PlaceObjects(FastNoise* noise)
{
	// Set meshes based on terrain type
	if (TerrainType == Forest) { StaticMeshes = ForestStaticMeshes; }
	else if (TerrainType == Snowy) { StaticMeshes = SnowyStaticMeshes; }
	else if (TerrainType == Piney) { StaticMeshes = PineyStaticMeshes; }
	else if (TerrainType == Mossy) { StaticMeshes = MossyStaticMeshes; }
	else if (TerrainType == Desert) { StaticMeshes = DesertStaticMeshes; }

	// Trees
	// For each vertex
	for (auto& vertex : Vertices)
	{
		// If the vertex is higher than the water level and noise result is greater than threshold
		if (vertex.Z > WaterLevel && noise->GetNoise(vertex.X, vertex.Y) > 0.5)
		{
			// Get a random mesh
			int meshNum = FMath::RandRange(0, StaticMeshes.Num() - 1);

			// Set location to the vertex position
			FTransform transform;
			transform.SetLocation(vertex);
			FQuat Rotation = FVector{ 0,0,0 }.ToOrientationQuat();
			transform.SetRotation(Rotation);

			if (meshNum >= 6)
			{
				// Scale Bushes
				transform.SetScale3D(FVector{ float(FMath::RandRange(0.8,1.0)) });
			}
			else
			{
				// Scale Trees
				transform.SetScale3D(FVector{ float(FMath::RandRange(2,3)) });
			}


			// Spawn actor
			FActorSpawnParameters params;
			auto object = GetWorld()->SpawnActor<ALandObject>(LandObjectClass, transform);
			auto staticMesh = StaticMeshes[meshNum];
			if (staticMesh)
			{			
				object->ObjectMesh = staticMesh;
				object->SpawnMesh(transform);
				LandObjects.Push(object);
			}

		}

		// Foliage

		// If terrain isnt too hot or too cold
		if (TerrainType != Desert && TerrainType != Snowy)
		{
			// Sample noise value and compare with threshold
			if (noise->GetNoise(vertex.X / 10, vertex.Y / 10) > 0.3)
			{
				if (vertex.Z > WaterLevel)
				{
					// Get random number to sample mesh list
					int meshNum = FMath::RandRange(0, FoliageStaticMeshes.Num() - 2);

					// Set location to vertex position and scale randomly
					FTransform transform;
					transform.SetLocation(vertex);
					FQuat Rotation = FVector{ 0,0,0 }.ToOrientationQuat();
					transform.SetRotation(Rotation);
					transform.SetScale3D(FVector{ float(FMath::RandRange(0.8,1.2)) });

					// Add instance of mesh in world with collision disabled
					auto staticMesh = FoliageStaticMeshes[meshNum];
					staticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					if (staticMesh) { staticMesh->AddInstance(transform); }
				}
				else
				{
					// Get vertex location and increase slightly
					auto location = vertex;
					location.Z = WaterLevel + 5;

					// Get lilypad mesh index
					int meshNum = FoliageStaticMeshes.Num() - 1;

					// Set location and scale randomly
					FTransform transform;
					transform.SetLocation(location);
					transform.SetScale3D(FVector{ float(FMath::RandRange(0.8,1.2)) });
					FQuat Rotation = FVector{ 0,0,0 }.ToOrientationQuat();
					transform.SetRotation(Rotation);

					// Add instance of lilypad into world with collision disabled
					auto staticMesh = FoliageStaticMeshes[meshNum];
					staticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					if (staticMesh) { staticMesh->AddInstance(transform); }
				}
			}
		}

	}

	// Create blockers
	for (int i = 0; i < 4; i++)
	{
		FTransform transform;

		// Set location to each side of mesh
		auto edgeLength = Size * Scale;
		FVector location;
		if (i == 0) { location = { edgeLength / 2,0,0 }; }
		if (i == 1) { location = { 0,edgeLength / 2,0 }; }
		if (i == 2) { location = { -edgeLength / 2,0,0 }; }
		if (i == 3) { location = { 0,-edgeLength / 2,0 }; }

		transform.SetLocation(location);

		// Set rotation to face inwards
		FQuat rotation;
		if (i == 0) { rotation = { 0, -0.707, 0, 0.707 }; }
		if (i == 1) { rotation = { 0, -0.707,  0.707, 0 }; }
		if (i == 2) { rotation = { 0, 0.707, 0, 0.707 }; }
		if (i == 3) { rotation = { 0, -0.707, -0.707, 0 }; }
		transform.SetRotation(rotation);
		transform.SetScale3D(FVector{ Scale, Scale, Scale });

		// Spawn actor and make invisible
		EdgeBlockers.Push(GetWorld()->SpawnActor<ABlocker>(BlockerClass, transform));
		EdgeBlockers[i]->SetActorHiddenInGame(true);
	}

}

void ALand::CreateMesh(FastNoise* noise)
{
	ABackLand::CreateMesh(noise);
	PlaceObjects(noise);	
}

void ALand::MakeNewMesh()
{
	// Create noise object, set noise type to fractal and set seed.
	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetSeed(FMath::RandRange(0, 999999999));

	CreateMesh(&noise);
}

void ALand::LoadStaticMeshes()
{
	ABackLand::LoadStaticMeshes();
	// Trees in first 6, Bushes after

	// Load asset and store in object
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset1(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset3(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset4(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset5(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset6(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset7(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset8(TEXT("StaticMesh'/Game/LowPolyAssets/BushBerries_1'"));

	// Create new instanced SMC and push into vector
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree1 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree2 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree3 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree4 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tree5 Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Stump Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Bush Static Mesh")));
	ForestStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Berry Bush Static Mesh")));

	// Pull mesh object from objectfinder and set to object created above
	ForestStaticMeshes[0]->SetStaticMesh(MeshAsset1.Object);
	ForestStaticMeshes[1]->SetStaticMesh(MeshAsset2.Object);
	ForestStaticMeshes[2]->SetStaticMesh(MeshAsset3.Object);
	ForestStaticMeshes[3]->SetStaticMesh(MeshAsset4.Object);
	ForestStaticMeshes[4]->SetStaticMesh(MeshAsset5.Object);
	ForestStaticMeshes[5]->SetStaticMesh(MeshAsset6.Object);
	ForestStaticMeshes[6]->SetStaticMesh(MeshAsset7.Object);
	ForestStaticMeshes[7]->SetStaticMesh(MeshAsset8.Object);

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset11(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset12(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset13(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset14(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset15(TEXT("StaticMesh'/Game/LowPolyAssets/CommonTree_Snow_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset16(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump_Snow'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset17(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_Snow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset18(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_Snow_2'"));

	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree1 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree2 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree3 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree4 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Tree5 Static Mesh")));
	SnowyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Snowy Stump Static Mesh")));
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

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset21(TEXT("StaticMesh'/Game/LowPolyAssets/PineTree_Autumn_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset22(TEXT("StaticMesh'/Game/LowPolyAssets/PineTree_Autumn_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset23(TEXT("StaticMesh'/Game/LowPolyAssets/PineTree_Autumn_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset24(TEXT("StaticMesh'/Game/LowPolyAssets/PineTree_Autumn_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset25(TEXT("StaticMesh'/Game/LowPolyAssets/PineTree_Autumn_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset26(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset27(TEXT("StaticMesh'/Game/LowPolyAssets/Wheat'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset28(TEXT("StaticMesh'/Game/LowPolyAssets/Rock_1'"));

	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree1 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree2 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree3 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree4 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Tree5 Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pine Stump Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wheat Static Mesh")));
	PineyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Rock Static Mesh")));

	PineyStaticMeshes[0]->SetStaticMesh(MeshAsset21.Object);
	PineyStaticMeshes[1]->SetStaticMesh(MeshAsset22.Object);
	PineyStaticMeshes[2]->SetStaticMesh(MeshAsset23.Object);
	PineyStaticMeshes[3]->SetStaticMesh(MeshAsset24.Object);
	PineyStaticMeshes[4]->SetStaticMesh(MeshAsset25.Object);
	PineyStaticMeshes[5]->SetStaticMesh(MeshAsset26.Object);
	PineyStaticMeshes[6]->SetStaticMesh(MeshAsset27.Object);
	PineyStaticMeshes[7]->SetStaticMesh(MeshAsset28.Object);

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset31(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset32(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset33(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset34(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset35(TEXT("StaticMesh'/Game/LowPolyAssets/Willow_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset36(TEXT("StaticMesh'/Game/LowPolyAssets/TreeStump'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset37(TEXT("StaticMesh'/Game/LowPolyAssets/Bush_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset38(TEXT("StaticMesh'/Game/LowPolyAssets/BushBerries_1'"));

	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree1 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree2 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree3 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree4 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Tree5 Static Mesh")));
	MossyStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Willow Stump Static Mesh")));
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


	// Last slot reserved for water foliage
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset51(TEXT("StaticMesh'/Game/LowPolyAssets/Grass'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset52(TEXT("StaticMesh'/Game/LowPolyAssets/Grass_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset53(TEXT("StaticMesh'/Game/LowPolyAssets/Grass_Short'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset54(TEXT("StaticMesh'/Game/LowPolyAssets/Grass_Large'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset55(TEXT("StaticMesh'/Game/LowPolyAssets/Grass_Small'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset56(TEXT("StaticMesh'/Game/LowPolyAssets/Plant_1'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset57(TEXT("StaticMesh'/Game/LowPolyAssets/Plant_2'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset58(TEXT("StaticMesh'/Game/LowPolyAssets/Plant_3'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset59(TEXT("StaticMesh'/Game/LowPolyAssets/Plant_4'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset60(TEXT("StaticMesh'/Game/LowPolyAssets/Plant_5'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset600(TEXT("StaticMesh'/Game/LowPolyAssets/Lilypad'"));

	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass 1 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass 2 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass 3 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass 4 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Grass 5 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Plant 1 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Plant 2 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Plant 3 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Plant 4 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Plant 5 Static Mesh")));
	FoliageStaticMeshes.Push(CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Lilypad Static Mesh")));

	FoliageStaticMeshes[0]->SetStaticMesh(MeshAsset51.Object);
	FoliageStaticMeshes[1]->SetStaticMesh(MeshAsset52.Object);
	FoliageStaticMeshes[2]->SetStaticMesh(MeshAsset53.Object);
	FoliageStaticMeshes[3]->SetStaticMesh(MeshAsset54.Object);
	FoliageStaticMeshes[4]->SetStaticMesh(MeshAsset55.Object);
	FoliageStaticMeshes[5]->SetStaticMesh(MeshAsset56.Object);
	FoliageStaticMeshes[6]->SetStaticMesh(MeshAsset57.Object);
	FoliageStaticMeshes[7]->SetStaticMesh(MeshAsset58.Object);
	FoliageStaticMeshes[8]->SetStaticMesh(MeshAsset59.Object);
	FoliageStaticMeshes[9]->SetStaticMesh(MeshAsset60.Object);
	FoliageStaticMeshes[10]->SetStaticMesh(MeshAsset600.Object);
}
