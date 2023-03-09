// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DecisionTask.h"
#include "AIController.h"
#include "AICharacter.h"

UBTTask_DecisionTask::UBTTask_DecisionTask()
{
	NodeName = "Making Decision";
}

EBTNodeResult::Type UBTTask_DecisionTask::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	if(AAICharacter* Character = Cast<AAICharacter>(Owner.GetAIOwner()->GetPawn()))
	{
		if(Character->Reasons == EDecisionReasons::None)
		{
			Character->State = EAIState::Patrol;
		}
		else if(Character->Reasons == EDecisionReasons::CoverNotFound)
		{
			// RUN State
		}
		else if(Character->Reasons == EDecisionReasons::BeingShot)
		{
			// Run or Find Cover or Shoot
			Character->State = EAIState::Shoot;
			
			// If health > 60
				// shoot

			// if health > 40
				// Find Cover

			// If health < 40
				// Run
			
		}
		else if(Character->Reasons == EDecisionReasons::PlayerNotFound)
		{
			// Run
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}