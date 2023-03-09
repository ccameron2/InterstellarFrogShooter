// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetState.h"
#include "AIController.h"

UBTTask_SetState::UBTTask_SetState()
{
	NodeName = "Set AI State";
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	if(AAICharacter* Character = Cast<AAICharacter>(Owner.GetAIOwner()->GetPawn()))
	{
		Character->State = State;
		Character->Reasons = Reason;
	}
	
	return EBTNodeResult::Succeeded;
}
