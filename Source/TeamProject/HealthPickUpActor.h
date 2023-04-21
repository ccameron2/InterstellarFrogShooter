// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpActor.h"
#include "HealthPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AHealthPickUpActor : public ABasePickUpActor
{
	GENERATED_BODY()

public:
	AHealthPickUpActor();

protected:

	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnPickUp(APlayerCharacter* Character) override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
		float HealthAmount = 10.0f;
};
