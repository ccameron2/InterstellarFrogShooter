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

	FVector NewLocation = OwnerComp.GetOwner()->GetActorLocation();
	NewLocation.X = FMath::RandRange(MinX, MaxX);
	NewLocation.Y = FMath::RandRange(MinY, MaxY);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);

}

