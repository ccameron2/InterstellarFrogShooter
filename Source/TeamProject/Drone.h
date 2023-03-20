// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/FloatingPawnMovement.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DroneMesh;
	UPROPERTY(EditAnywhere)
		UFloatingPawnMovement* FloatingComponent;

	UPROPERTY(EditAnywhere)
		float MaxDistance = 1000.0f;

	bool Up = true;
	FTimerHandle WobbleTimer;
	float WobbleChangeTime = 1.0f;
	void DirTimerUp();
};
