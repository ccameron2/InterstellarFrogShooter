// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickUpActor.generated.h"

class APlayerCharacter;
class USphereComponent;

// Enum for setting the pick up types
// Used in blueprints (see AICharacter.h)
UENUM()
enum class EPickUpType
{
	None = 0,
	Health,
	Damage,
	Speed,
};

UCLASS()
class TEAMPROJECT_API ABasePickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void OnPickUp(APlayerCharacter* Character);
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

	
public:	
	

	EPickUpType GetPickUpType();

private:
	void OnLifeTimerFinished();
	
protected:
	// Each setting changeable by child classes
	UPROPERTY(EditAnywhere)
		EPickUpType PickUpType;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		USphereComponent* PickUpCollisionSphere; // Sphere for collision detection for picking up

	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent; // Plays audio on pick up

	// stops the Life timer timer from destroying pick up if already picked up
	bool IsPickedUp = false;
	
private:
	// Default settings
	UPROPERTY(EditAnywhere)
		float RotationSpeed = 100.0f; // The speed that the pick ups will spin at

	UPROPERTY(EditAnywhere)
		float LifeTimeRate = 120.0f; //Timer used for destroying after 2 minutes if not picked up (done for performance)
	
	FTimerHandle LifeTimeTimer;

};
