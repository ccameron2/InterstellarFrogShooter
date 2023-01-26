//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATestActor::ATestActor()
{
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	BoxMesh->SetupAttachment(RootComponent);
	SetRootComponent(BoxMesh);

}

void ATestActor::HitByPlayer()
{
	FVector SpawnLocation = GetActorLocation();


	SpawnLocation.X += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Y += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Z += 150.0f;

	FRotator SpawnRotation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorRotation();



	GetWorld()->SpawnActor<AHitpointText>(HitPointText, SpawnLocation, SpawnRotation);
}


