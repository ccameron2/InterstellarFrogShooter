// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetRunLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTask_SetRunLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetRunLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;
};
