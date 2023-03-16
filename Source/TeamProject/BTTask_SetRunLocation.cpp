// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRunLocation.h"
#include "AIController.h"
#include "AICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetRunLocation::UBTTask_SetRunLocation()
{
	NodeName = "Set Run Location";
}

EBTNodeResult::Type UBTTask_SetRunLocation::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	Super::ExecuteTask(Owner, NodeMemory);
	
	if(AAICharacter* Character = Cast<AAICharacter>(Owner.GetAIOwner()->GetPawn()))
	{
		TArray<AActor*> OutActors;
		AAIController* Controller = Owner.GetAIOwner();
		if(Controller->PerceptionComponent)
		{
			Controller->PerceptionComponent->GetPerceivedHostileActors(OutActors);
		}
		
		// Check players position
		AActor* Player = nullptr;
		if(!OutActors.IsEmpty())
		{
			for(auto Actor : OutActors) // Will only ever be one
			{
				Player = Actor;
			}
		}

		FVector RunLocation;
		// If can see player set position to be behind
		if(Player != nullptr)
		{
			RunLocation = Character->GetActorLocation() + -Character->GetActorForwardVector() * Character->RunAwayDistance;
		}
		else // else just run
		{
			RunLocation = Character->GetActorLocation() + Character->GetActorForwardVector() * Character->RunAwayDistance;
		}
		Owner.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RunLocation);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
