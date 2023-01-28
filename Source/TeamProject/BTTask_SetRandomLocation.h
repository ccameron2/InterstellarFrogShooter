// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTask_SetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_SetRandomLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;

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
