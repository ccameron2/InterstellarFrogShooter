// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayer.h"

#include "AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

class AAICharacter;

UBTTask_FindPlayer::UBTTask_FindPlayer()
{
	NodeName = "Find Player Shot Location";
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	Super::ExecuteTask(Owner, NodeMemory);

	// Sets the location where the AI was shot from
	if(AAICharacter* Character = Cast<AAICharacter>(Owner.GetAIOwner()->GetPawn()))
	{
		Owner.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Character->ShootFromLocation);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
