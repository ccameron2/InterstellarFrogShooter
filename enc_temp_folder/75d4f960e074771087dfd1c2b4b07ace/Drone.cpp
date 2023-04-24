// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Mesh"));
	SetRootComponent(DroneMesh);
	DroneMesh->SetRelativeRotation(FVector{ 0,0,90 }.ToOrientationQuat());
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

	if (UnlockedWeapon)
	{
		if (WeaponOnCooldown) return;

		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), outActors);

		float minDistance = INT_MAX;
		auto location = GetActorLocation();
		AActor* closestEnemy = nullptr;
		for (auto& actor : outActors)
		{
			auto actorLocation = actor->GetActorLocation();
			auto actorDistance = FVector::Distance(location, actorLocation);
			if (actorDistance < minDistance)
			{
				minDistance = actorDistance;
				closestEnemy = actor;
			}
		} 

		if (minDistance > AttackRange) return;

		FRotator rotation;
		GetActorEyesViewPoint(location, rotation);

		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		auto owner = GetOwner();
		if(owner) params.AddIgnoredActor(owner);
		bool bHit;
		if(closestEnemy) bHit = GetWorld()->LineTraceSingleByChannel(hitResult, location, closestEnemy->GetActorLocation(), ECollisionChannel::ECC_Pawn, params);

		if (bHit)
		{
			auto hitActor = hitResult.GetActor();
			if (hitActor)
			{
				UGameplayStatics::ApplyDamage(
					hitActor,
					DroneDamage, // Damage Amount 
					GetInstigatorController(),
					this,
					UDamageType::StaticClass());
			}
		}
									 
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &ADrone::WeaponCooldownUp, WeaponCooldownTime, false);
		WeaponOnCooldown = true;
	}

}

void ADrone::WeaponCooldownUp()
{
	WeaponOnCooldown = false;
}

void ADrone::DirTimerUp()
{
	Up = !Up;
}
