// CCameron
#pragma once

#include <ProceduralMeshComponent.h>
#include "Components/InstancedStaticMeshComponent.h" 
#include "Blocker.h"
#include "LandObject.h"
#include "External/FastNoise.h"


#include "BackLand.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Land.generated.h"

UCLASS()
class TEAMPROJECT_API ALand : public ABackLand
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALand();

	void Init(int type, FastNoise* noise) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Obsticles in the arena
	UPROPERTY(VisibleAnywhere, Category = "ProcGen")
		TArray<ALandObject*> LandObjects;

	// Class of obstacles
	UPROPERTY(EditAnywhere)
		TSubclassOf<ALandObject> LandObjectClass;

	// Currently set meshes
	TArray<UInstancedStaticMeshComponent*> StaticMeshes;

	// Blockers for arena edges
	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TSubclassOf<ABlocker> BlockerClass;
	UPROPERTY(EditAnywhere, Category = "ProcGen")
		TArray<ABlocker*> EdgeBlockers;

	// Static meshes for each world type
	TArray<UInstancedStaticMeshComponent*> ForestStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> SnowyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> MossyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> PineyStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> DesertStaticMeshes;
	TArray<UInstancedStaticMeshComponent*> FoliageStaticMeshes;

	UPROPERTY(EditAnywhere, Category = "ProcGen")
		bool CreateOnConstruction = false;

	void PlaceObjects(FastNoise* noise);
	void CreateMesh(FastNoise* noise) override;
	void ClearMeshInstances();
	void Clear() override;

	int NumTypes = 5;

	// Editor mesh creation function
	UFUNCTION(CallInEditor, Category = "ProcGen")
		void MakeNewMesh();

	// Load meshes from file
	void LoadStaticMeshes() override;

};
