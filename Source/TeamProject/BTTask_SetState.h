// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AICharacter.h"
#include "BTTask_SetState.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTask_SetState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;

private:
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="AI State Enum"))
		EAIState State;
	
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="AI Reson Enum"))
		EDecisionReasons Reason;
};
