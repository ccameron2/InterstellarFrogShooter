// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindCover.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UBTTask_FindCover : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FindCover();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;

	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void CalculateClosestActor(APawn* Owner);
	
private:
	UPROPERTY()
		TArray<AActor*> OutActors;
		
	UPROPERTY()
		float CurrentDistance;

	
		FVector CoverLocation;

	UPROPERTY()
		AActor* CoverActor;
};
