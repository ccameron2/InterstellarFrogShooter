// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerInSight.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTService_PlayerInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_PlayerInSight();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; 
	
};
