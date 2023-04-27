// CCameron

#include "Rocket.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARocket::ARocket()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create rocket mesh
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketMesh"));
	RocketMesh->SetSimulatePhysics(true);
	SetRootComponent(RocketMesh);

	// Create movement component and set values
	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ProjMovement->UpdatedComponent = RocketMesh;
	ProjMovement->InitialSpeed = 4000.0f;
	ProjMovement->MaxSpeed = 4000.0f;
	ProjMovement->bRotationFollowsVelocity = true;
}

void ARocket::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherActor != this->GetParentActor())
	{
		// Apply a radial force to push objects away from the explosion
		FVector ExplosionLocation = GetActorLocation();	

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, 0.0f, ExplosionLocation, ExplosionRadius, ExplosionRadius, 1.0f, nullptr, TArray<AActor*>(), this, this->GetInstigatorController());

		if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
		{
			// Apply a force to the other actor
			FVector ForceDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			OtherActor->FindComponentByClass<UStaticMeshComponent>()->AddImpulse(ForceDirection * ImpulseStrength, NAME_None, true);
		}
		
		// Spawn particles and play sound
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation(),3.0f);

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

