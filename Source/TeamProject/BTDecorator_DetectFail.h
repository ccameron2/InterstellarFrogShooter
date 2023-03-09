// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AICharacter.h"
#include "BTDecorator_DetectFail.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTDecorator_DetectFail : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_DetectFail();

protected:
	virtual void OnNodeProcessed(struct FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) override;

public:
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="AI Reset State"))
		EAIState State;

	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="AI Reset Reason"))
		EDecisionReasons Reason;
};
