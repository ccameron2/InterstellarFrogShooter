// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpActor.h"
#include "SpeedPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ASpeedPickUpActor : public ABasePickUpActor
{
	GENERATED_BODY()

public:
	ASpeedPickUpActor();

protected:
	virtual void OnPickUp(APlayerCharacter* Character) override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult) override;

private:
	void OnTimerFinished();

private:
	float PreviousSpeed;

	UPROPERTY(EditAnywhere)
		float SpeedMultiplier = 2.0f;
	
	UPROPERTY(EditAnywhere)
		float PickUpTimer = 10.0f;
	
	FTimerHandle SpeedTimer;
	
	UPROPERTY()
		class APlayerCharacter* PlayerCharacter;
};
