// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DetectFail.h"


#include "AIController.h"

UBTDecorator_DetectFail::UBTDecorator_DetectFail()
{
	NodeName = "Detect Fail";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UBTDecorator_DetectFail::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	//Super::OnNodeProcessed(SearchData, NodeResult);
	
	if(NodeResult == EBTNodeResult::Failed)
	{
		if(AAICharacter* Character = Cast<AAICharacter>(SearchData.OwnerComp.GetAIOwner()->GetPawn()))
		{
			Character->State = State;
			Character->Reasons = Reason;
		}
	}
	
}

