// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"


UBTService_SetRandomLocation::UBTService_SetRandomLocation()
{
	NodeName = TEXT("SetRandomLocation");
}

void UBTService_SetRandomLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ChooseWaypoint()->GetActorLocation());

}

AActor* UBTService_SetRandomLocation::ChooseWaypoint()
{
	int index = FMath::RandRange(0, Waypoints.Num() - 1); // Finds the random waypoint 
	return Waypoints[index]; // Returns the random value
}
