// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DetectFail.h"


#include "AIController.h"
#include "BehaviorTree/BTCompositeNode.h"

UBTDecorator_DetectFail::UBTDecorator_DetectFail()
{
	NodeName = "Detect Fail";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UBTDecorator_DetectFail::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);

	// If one of the children nodes fail then this is called to reset the states and
	// stop the states getting stuck
	if(NodeResult == EBTNodeResult::Failed)
	{
		if(AAICharacter* Character = Cast<AAICharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn()))
		{
			if(Character->Reasons == EDecisionReasons::Clear) // Only Changes the reason if cleared
			{
				Character->State = State;
				Character->Reasons = Reason;
			}
		}
	}
	
}

