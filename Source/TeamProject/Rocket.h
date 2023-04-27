// CCameron

#pragma once

#include "GameFramework/ProjectileMovementComponent.h" 
#include "Components/SphereComponent.h" 
#include "Sound/SoundCue.h" 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class TEAMPROJECT_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocket();

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* RocketMesh;

	// Projectile movement component
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjMovement;

	// Explosion particles 
	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionEffect;

	// Explosion sound
	UPROPERTY(EditAnywhere)
		USoundCue* ExplosionSound;

	// Damage dealt
	UPROPERTY(EditAnywhere)
		float Damage = 75;

	// Radius of explosion
	UPROPERTY(EditAnywhere)
		float ExplosionRadius = 500.0f;
	
	// Impulse to apply to enemies hit
	UPROPERTY(EditAnywhere)
		float ImpulseStrength = 100000.0f;
};
