// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTService_SetRandomLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_SetRandomLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:

	UPROPERTY(EditAnywhere)
		float MaxX;

	UPROPERTY(EditAnywhere)
		float MinX;

	UPROPERTY(EditAnywhere)
		float MaxY;

	UPROPERTY(EditAnywhere)
		float MinY;
};
