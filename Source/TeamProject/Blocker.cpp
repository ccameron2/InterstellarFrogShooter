// CCameron

#include "Blocker.h"

// Sets default values
ABlocker::ABlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh to block player
	BlockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMesh"));
	BlockerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(BlockerMesh);
}

// Called when the game starts or when spawned
void ABlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

