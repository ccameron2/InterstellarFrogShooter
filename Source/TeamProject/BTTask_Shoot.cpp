// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AICharacter.h"
#include "AIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
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
	
		AActor* Target = nullptr; 
		if(!OutActors.IsEmpty())
		{
			for(auto actor : OutActors)
			{
				Target = actor;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
		
		Character->Shoot(Target);
	}
	else
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
