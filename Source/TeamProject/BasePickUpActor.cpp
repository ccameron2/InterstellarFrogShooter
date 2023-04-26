// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickUpActor.h"

#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABasePickUpActor::ABasePickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates the components
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Up Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pick Up Col Sphere"));
	PickUpCollisionSphere->SetupAttachment(MeshComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("PickUpAudio");
}

// Called when the game starts or when spawned
void ABasePickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
	// binds the overlap function
	PickUpCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABasePickUpActor::OnBeginOverlap);

	// Starts the life timer when spawned
	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimer, this, &ABasePickUpActor::OnLifeTimerFinished, LifeTimeRate, false);
}

void ABasePickUpActor::OnPickUp(APlayerCharacter* Character)
{
	// Overridable pick up function, this is where all the pickup code is in the children classes
}

void ABasePickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
	{
		// Only picks up if the overlapping character is the player 
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
	return PickUpType; // Getter for getting the pickup type
}

void ABasePickUpActor::OnLifeTimerFinished()
{
	if(!IsPickedUp)
		Destroy(); // Destroys actor if not picked up
}

