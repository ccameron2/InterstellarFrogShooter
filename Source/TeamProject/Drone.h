// CCameron

#pragma once
#include "GameFramework/FloatingPawnMovement.h"
#include "CoreMinimal.h"
#include "Components/SphereComponent.h" 

#include "GameFramework/Pawn.h"
#include "AICharacter.h"
#include "Drone.generated.h"

UCLASS()
class TEAMPROJECT_API ADrone : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void WeaponCooldownUp();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditAnywhere)
		UFloatingPawnMovement* FloatingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DroneDamage = 10.0f;

	// Has player unlocked drone weapon skill
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UnlockedWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WeaponCooldownTime = 2.0f;
	
	// Weapon cooldown timer
	bool WeaponOnCooldown = false;
	FTimerHandle WeaponCooldownTimer;

	bool Up = true;
	FTimerHandle WobbleTimer;
	float WobbleChangeTime = 1.0f;

	// Change hover direction
	void DirTimerUp();
};
