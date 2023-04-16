// Fill out your copyright notice in the Description page of Project Settings.

#include "Rocket.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetSimulatePhysics(true);

	SetRootComponent(RocketMesh);

	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjMovement->UpdatedComponent = RocketMesh;
	ProjMovement->InitialSpeed = 4000.0f;
	ProjMovement->MaxSpeed = 4000.0f;
	ProjMovement->bRotationFollowsVelocity = true;
}

void ARocket::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Apply a radial force to push objects away from the explosion
		FVector ExplosionLocation = GetActorLocation();
		TArray<FHitResult> HitResults;
		float mExplosionRadius = 500.0f;
		float mImpulseStrength = 100000.0f;
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, 0.0f, ExplosionLocation, mExplosionRadius, mExplosionRadius, 1.0f, nullptr, TArray<AActor*>(), this, this->GetInstigatorController());

		if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
		{
			// Apply a force to the other actor
			FVector ForceDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			OtherActor->FindComponentByClass<UStaticMeshComponent>()->AddImpulse(ForceDirection * mImpulseStrength, NAME_None, true);
		}		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
		Destroy();
	}
}


// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	ProjMovement->Activate();
	SetLifeSpan(5.0f);
	OnActorHit.AddDynamic(this, &ARocket::OnHit);
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

