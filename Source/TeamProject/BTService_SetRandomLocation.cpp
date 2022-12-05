// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SetRandomLocation::UBTService_SetRandomLocation()
{
	NodeName = TEXT("SetRandomLocation");
}

void UBTService_SetRandomLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FVector(1000.0f, 10.0f, 100.0f));

}
