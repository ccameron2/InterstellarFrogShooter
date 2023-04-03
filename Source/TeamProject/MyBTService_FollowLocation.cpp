// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_FollowLocation.h"

#include "AICharacter.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_FollowLocation::UMyBTService_FollowLocation()
{
	NodeName = "Follow Player Location";
}

void UMyBTService_FollowLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	
	if(AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		TArray<AActor*> OutActors;
		AAIController* Controller = OwnerComp.GetAIOwner();
		if(Controller->PerceptionComponent)
		{
			Controller->PerceptionComponent->GetPerceivedHostileActors(OutActors);
		}

		
		if(!OutActors.IsEmpty())
		{
			for(auto actor : OutActors)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), actor->GetActorLocation());
				break; // There is only one hostile actor so should instantly leave after finding
			}
		}

	}
	
}
