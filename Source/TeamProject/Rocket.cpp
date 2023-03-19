// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	SetRootComponent(RocketMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereCollision->SetupAttachment(RocketMesh);
	SphereCollision->OnComponentHit.AddDynamic(this, &ARocket::OnHit);

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjMovement->UpdatedComponent = RocketMesh;
	ProjMovement->InitialSpeed = 4000.0f;
	ProjMovement->MaxSpeed = 4000.0f;
	ProjMovement->bRotationFollowsVelocity = true;
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
	}
}


// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	ProjMovement->Activate();
	SetLifeSpan(5.0f);
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

