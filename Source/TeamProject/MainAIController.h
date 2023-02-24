// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "MainAIController.generated.h"

class UAISense;

UCLASS()
class TEAMPROJECT_API AMainAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMainAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditAnywhere)
		UBehaviorTree* AIBehaviour;

	UPROPERTY(EditAnywhere)
		class UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditAnywhere)
		FGenericTeamId TeamId = 1;

	UPROPERTY()
		TArray<AActor*> OutActors;
};
