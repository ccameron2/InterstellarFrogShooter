// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateState.h"
#include "AIController.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateState::UBTService_UpdateState()
{
	NodeName = "Update AI State";
}

void UBTService_UpdateState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	// Allows the AI Blackboard state to be updated by current ai state, which causes the current ai state decorator to abort the sequence
	// If the blackboard gets updated
	
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(Character->State));
}
