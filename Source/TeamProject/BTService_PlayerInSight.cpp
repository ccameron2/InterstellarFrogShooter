// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerInSight.h"

#include "AICharacter.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"

UBTService_PlayerInSight::UBTService_PlayerInSight()
{
	NodeName = "Check if can see player";
}

void UBTService_PlayerInSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Uses the perception component(eyes) to check if the player is in the AIs view (see MainAIController.cpp)
	if(AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		
		TArray<AActor*> OutActors;
		AAIController* Controller = OwnerComp.GetAIOwner();
		if(Controller->PerceptionComponent)
		{
			// Get all characters in sight (only checking for one which is the player)
			Controller->PerceptionComponent->GetPerceivedHostileActors(OutActors);
		}

		// If the player is found change the state and reason
		// This will cause the AI to start attacking the player
		if(!OutActors.IsEmpty())
		{
			Character->State = EAIState::Shoot;
			Character->Reasons = EDecisionReasons::BeingShot; // The reason used to change to the shoot 
		}

	}
}
