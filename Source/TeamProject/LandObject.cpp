// CCameron

#include "LandObject.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ALandObject::ALandObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//ObjectMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Object Mesh");
	
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));

	// Does not work in C++, needs to be done in the blueprint  (https://forums.unrealengine.com/t/cant-register-uaisense-sight-with-uaiperceptionstimulisourcecomponent/149852)
	//StimuliSourceComponent->bAutoRegister = true;
	//StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());

	Tags.Add("Cover");
}

// Called when the game starts or when spawned
void ALandObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALandObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALandObject::SpawnMesh(FTransform transform)
{
	if (ObjectMesh) { ObjectMesh->AddInstance(transform); }
}

