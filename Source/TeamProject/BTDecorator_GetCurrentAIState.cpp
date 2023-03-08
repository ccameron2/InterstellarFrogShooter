// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_GetCurrentAIState.h"
#include "AIController.h"

UBTDecorator_GetCurrentAIState::UBTDecorator_GetCurrentAIState()
{
	NodeName = "Current AI State";
}

bool UBTDecorator_GetCurrentAIState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(Character)
	{
		if(State == Character->State)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// EBlackboardNotificationResult UBTDecorator_GetCurrentAIState::OnBlackboardKeyValueChange(
// 	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("BlackBoard Value Changed"));
// 	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
// }
//
// void UBTDecorator_GetCurrentAIState::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
// 	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
// {
// 	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
// }

FString UBTDecorator_GetCurrentAIState::GetStaticDescription() const
{
	return FString::Printf(TEXT("State being checked: %s"), *UEnum::GetValueAsString(State));
}
