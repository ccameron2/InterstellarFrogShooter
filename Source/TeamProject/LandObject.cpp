// Fill out your copyright notice in the Description page of Project Settings.


#include "LandObject.h"

// Sets default values
ALandObject::ALandObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//ObjectMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Object Mesh");
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

