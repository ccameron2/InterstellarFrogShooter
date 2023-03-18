// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_FollowLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UMyBTService_FollowLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UMyBTService_FollowLocation();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; 
};
