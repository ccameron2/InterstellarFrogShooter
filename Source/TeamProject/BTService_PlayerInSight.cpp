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

	//UE_LOG(LogTemp, Warning, TEXT("TICKKKing"));

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
			Character->State = EAIState::Shoot;
			Character->Reasons = EDecisionReasons::BeingShot; // The reason used to change to the shoot 
		}

	}
}
