// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindCover.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"



UBTTask_FindCover::UBTTask_FindCover()
{
	NodeName = TEXT("Find Cover");
	//INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_FindCover::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	AAIController* Controller = Owner.GetAIOwner();
	if(Controller->PerceptionComponent)
	{
		Controller->PerceptionComponent->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), OutActors);
		CalculateClosestActor(Controller->GetPawn());
		//Owner.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), CoverLocation);
		if(CoverActor != nullptr)
		{
			Owner.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), CoverActor);
			Owner.GetBlackboardComponent()->SetValueAsVector("CoverLocation", CoverLocation);
		}
	}
	
	return EBTNodeResult::Succeeded;
}

void UBTTask_FindCover::CalculateClosestActor(APawn* Owner)
{
	CurrentDistance = -1;
	CoverActor = nullptr;
	for(auto actor : OutActors)
	{
		if(actor->ActorHasTag("Cover"))
		{
			float distance = FVector::Distance(Owner->GetActorLocation(), actor->GetActorLocation());
			if(CurrentDistance == -1)
			{
				CurrentDistance = distance;
				//CoverLocation = actor->GetActorLocation();
				CoverActor = actor;
			}
			else
			{
				if(CurrentDistance > distance)
				{
					CurrentDistance = distance;
					//CoverLocation = actor->GetActorLocation();
					CoverActor = actor;
				}
			}
		}
	}

	if(CoverActor != nullptr)
	{
		CoverLocation = CoverActor->GetActorLocation();
	}
}

//oid UBTTask_FindCover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//
//	UE_LOG(LogTemp, Warning, TEXT("Find Cover tick"));
//	//if()
//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//
