// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAIController.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AMainAIController::AMainAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SetPerceptionComponent(*AIPerception);

	// Perception component settings
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 3500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Sets the AI team ID
	SetGenericTeamId(TeamId);
}

void AMainAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviour != nullptr)
	{
		// Runs the behaviour tree to allow the AI to work
		RunBehaviorTree(AIBehaviour);
	}

	// Binds the Perception functions
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AMainAIController::OnPerceptionUpdated);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::OnTargetPerceptionUpdated);
	
}

void AMainAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ETeamAttitude::Type AMainAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// Sets the teams
	if (const APawn* pawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(pawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == TeamId)
			{
				return ETeamAttitude::Friendly;
			}
		}
	}
	if (const APlayerCharacter* pawn = Cast<APlayerCharacter>(&Other))
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}

void AMainAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		ETeamAttitude::Type type = GetTeamAttitudeTowards(*Actor);
		if(type == ETeamAttitude::Friendly) 
		{
			//UE_LOG(LogTemp, Warning, TEXT("Friendly Actor: %s"), *Actor->GetName());
		}
		else if(type == ETeamAttitude::Hostile)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hostile Actor: %s"), *Actor->GetName());
		}
		else if(type == ETeamAttitude::Neutral)
		{
			if(Actor->ActorHasTag("Cover"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Cover Actor: %s"), *Actor->GetName());
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Neutral Actor: %s"), *Actor->GetName());
			}
		}
	}

}

void AMainAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}