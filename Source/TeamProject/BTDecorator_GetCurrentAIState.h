// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "AICharacter.h"
#include "BTDecorator_GetCurrentAIState.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTDecorator_GetCurrentAIState : public UBTDecorator_Blackboard
{
	GENERATED_BODY()


public:
	UBTDecorator_GetCurrentAIState();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="AI State Enum"))
		EAIState State;
};
