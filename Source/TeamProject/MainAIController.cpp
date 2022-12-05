// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAIController.h"

void AMainAIController::BeginPlay()
{
	if (AIBehaviour != nullptr)
		RunBehaviorTree(AIBehaviour);
}

void AMainAIController::Tick(float DeltaTime)
{
}
