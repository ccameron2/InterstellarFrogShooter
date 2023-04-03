// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickUpActor.h"

#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ABasePickUpActor::ABasePickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Up Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Col Sphere"));
	PickUpCollisionSphere->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ABasePickUpActor::BeginPlay()
{
	Super::BeginPlay();
	PickUpCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePickUpActor::OnBeginOverlap);
}

void ABasePickUpActor::OnPickUp(APlayerCharacter* Character)
{
	
}

void ABasePickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
		OnPickUp(Character);
	}
}

// Called every frame
void ABasePickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ROTATE PICKUP
	FRotator Rotator(0, 1, 0);
	AddActorLocalRotation((Rotator * RotationSpeed) * DeltaTime);
}

EPickUpType ABasePickUpActor::GetPickUpType()
{
	return PickUpType;
}

