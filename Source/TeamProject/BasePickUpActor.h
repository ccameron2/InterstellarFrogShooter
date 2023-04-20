// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickUpActor.generated.h"

class APlayerCharacter;
class USphereComponent;

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

	virtual void OnPickUp(APlayerCharacter* Character);
	
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EPickUpType GetPickUpType();
	

protected:
	UPROPERTY(EditAnywhere)
		EPickUpType PickUpType;
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		USphereComponent* PickUpCollisionSphere;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;
	
private:
	UPROPERTY(EditAnywhere)
		float RotationSpeed = 100.0f;
	
	

};
