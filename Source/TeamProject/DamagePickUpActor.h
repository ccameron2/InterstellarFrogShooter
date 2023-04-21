// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpActor.h"
#include "DamagePickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ADamagePickUpActor : public ABasePickUpActor
{
	GENERATED_BODY()

public:
	ADamagePickUpActor();

protected:
	virtual void OnPickUp(APlayerCharacter* Character) override;

	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult) override;

private:
	void OnTimerFinished();
	
private:
	int PreviousDamage;

	UPROPERTY(EditAnywhere)
		int DamageMultiplier = 2;

	UPROPERTY(EditAnywhere)
		float PickUpTimer = 20.0f;
	
	FTimerHandle PowerUpTimer;

	UPROPERTY()
		class APlayerCharacter* PlayerCharacter;
};
