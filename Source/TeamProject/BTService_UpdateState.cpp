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

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(Character->State));
	EAIState state = static_cast<EAIState>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(GetSelectedBlackboardKey()));
}
