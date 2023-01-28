// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetRandomLocation::UBTTask_SetRandomLocation()
{
	NodeName = TEXT("Set Random Location Task");
}

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	Super::ExecuteTask(Owner, NodeMemory);

	FVector NewLocation = Owner.GetOwner()->GetActorLocation();
	NewLocation.X = FMath::RandRange(MinX, MaxX);
	NewLocation.Y = FMath::RandRange(MinY, MaxY);

	Owner.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);

	return EBTNodeResult::Succeeded;
}
