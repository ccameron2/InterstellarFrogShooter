// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AMainAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehaviour != nullptr)
		RunBehaviorTree(AIBehaviour);
}

void AMainAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("AI Controller ticking"));
}
