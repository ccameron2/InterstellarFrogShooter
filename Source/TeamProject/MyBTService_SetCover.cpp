// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_SetCover.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_SetCover::UMyBTService_SetCover()
{
	NodeName = TEXT("Set Cover");
}

void UMyBTService_SetCover::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Sets the cover location to be behind the cover
	AActor* actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if(actor != nullptr)
	{
		FVector End = (OwnerComp.GetAIOwner()->GetPawn()->GetActorForwardVector() * 200.0f) + actor->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("CoverLocation", End);
	}
}

