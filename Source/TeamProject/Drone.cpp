// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Mesh"));
	SetRootComponent(DroneMesh);

	FloatingComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(WobbleTimer, this, &ADrone::DirTimerUp, WobbleChangeTime, true);
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Up) SetActorLocation(GetActorLocation() + FVector{ 0,0,0.5 });
	else SetActorLocation(GetActorLocation() + FVector{ 0,0,-0.5 });
}

void ADrone::DirTimerUp()
{
	Up = !Up;
}
