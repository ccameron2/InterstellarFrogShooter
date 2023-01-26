//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HitpointText.h"

#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class TEAMPROJECT_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	UFUNCTION()
		void HitByPlayer();

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AHitpointText> HitPointText;


};
