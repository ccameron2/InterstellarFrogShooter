// CCameron


#include "Drone.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(SphereCollision);

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Mesh"));
	DroneMesh->SetRelativeRotation(FVector{ 0,0,90 }.ToOrientationQuat());
	DroneMesh->SetupAttachment(RootComponent);

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

	// Move drone up and down
	if (Up) SetActorLocation(GetActorLocation() + FVector{ 0,0,0.5 });
	else SetActorLocation(GetActorLocation() + FVector{ 0,0,-0.5 });

	// If drone weapon unlocked
	if (UnlockedWeapon)
	{
		if (WeaponOnCooldown) return;

		// Get all enemies
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), outActors);

		// Find the closest one
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

		// If they are in attack range
		if (minDistance > AttackRange) return;

		// Get viewpoint of drone
		FRotator rotation;
		GetActorEyesViewPoint(location, rotation);

		// Raycast from drone viewpoint to closest enemy
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		auto owner = GetOwner();

		if(owner) params.AddIgnoredActor(owner);
		bool bHit;
		if(closestEnemy) bHit = GetWorld()->LineTraceSingleByChannel(hitResult, location, closestEnemy->GetActorLocation(), ECollisionChannel::ECC_Pawn, params);

		// If hit apply damage
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
						
		// Set weapon cooldown timer
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
