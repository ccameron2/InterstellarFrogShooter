// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/InstancedStaticMeshComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandObject.generated.h"

UCLASS()
class TEAMPROJECT_API ALandObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* ObjectMesh;

	void SpawnMesh(FTransform transform);

};
