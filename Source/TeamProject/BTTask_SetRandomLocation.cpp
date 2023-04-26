// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomLocation.h"

#include "AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetRandomLocation::UBTTask_SetRandomLocation()
{
	NodeName = TEXT("Set Random Location Task");
}

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	Super::ExecuteTask(Owner, NodeMemory);

	// Set random location in world bounds
	// This is for patrolling randomly
	FVector NewLocation = Owner.GetOwner()->GetActorLocation();
	if(AAICharacter* Character = Cast<AAICharacter>(Owner.GetAIOwner()->GetPawn()))
	{
		NewLocation.X = FMath::RandRange(Character->MinWorldX, Character->MaxWorldX);
		NewLocation.Y = FMath::RandRange(Character->MinWorldY, Character->MaxWorldY);
	}

	Owner.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);
	
	return EBTNodeResult::Succeeded;
}
