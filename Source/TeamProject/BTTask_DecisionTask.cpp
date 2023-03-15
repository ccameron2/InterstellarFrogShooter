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
		
		UE_LOG(LogTemp, Warning, TEXT("Making new Decision %s"), *UEnum::GetValueAsString(Character->Reasons));
		if(Character->Reasons == EDecisionReasons::None)
		{
			Character->State = EAIState::Patrol;
		}
		else if(Character->Reasons == EDecisionReasons::CoverNotFound)
		{
			Character->State = EAIState::Run;
		}
		else if(Character->Reasons == EDecisionReasons::BeingShot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Making new Decision"));
			// Run or Find Cover or Shoot
			Character->State = EAIState::Shoot;
			
			if (Character->Health > 60)
				Character->State = EAIState::Shoot;
			
			if (Character->Health  > 40 && Character->Health < 60)
			 	Character->State = EAIState::FindCover;

			if (Character->Health < 40)
				Character->State = EAIState::Run;
			
		}
		else if(Character->Reasons == EDecisionReasons::PlayerNotFound)
		{
			Character->State = EAIState::Run;
		}

		Character->Reasons = EDecisionReasons::Clear; // Clears the reason
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}