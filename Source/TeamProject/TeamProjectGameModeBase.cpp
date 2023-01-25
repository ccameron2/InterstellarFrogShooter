// Copyright Epic Games, Inc. All Rights Reserved.
#include "TeamProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ATeamProjectGameModeBase::ATeamProjectGameModeBase()
{
	
}

void ATeamProjectGameModeBase::BeginPlay()
{
	PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerPawn = PlayerController->GetPawn();

	PlayerPawn->SetActorLocation({0, 10000, 40000 });

	if (LandClass)
	{
		FTransform Transform;
		LandActor = GetWorld()->SpawnActor<ALand>(LandClass, Transform);
		FVector Location = { 0,0,0 };
		Transform.SetTranslation(Location);
	}
}

void ATeamProjectGameModeBase::OnGuiSetValues(FText InSeedString, int InTerrainType)
{
	//Initialise a seed for the case when everything fails with generating a seed
	int seed = 89514156;

	//Convert the entered Text into a String variable
	FString seedString = InSeedString.ToString();

	//Check if the String is numeric
	//Needs to be checked as this ignores any non numerical characters that the player enters
	if (seedString.IsNumeric())
	{
		//convert the String to an Int
		int32 seedInt = FCString::Atoi(*seedString);

		//Set the seed to the converted Int
		seed = seedInt;
	}
	else
	{
		//if the player enters text with any non numerical character 
		//Generate a random seed
		seed = rand();
	}

	//Currently log the generated Seed and the chosen Terrain Type for Debuging
	UE_LOG(LogTemp, Warning, TEXT("SEED: %d"), seed);
	UE_LOG(LogTemp, Warning, TEXT("TERRAIN TYPE: %d"), InTerrainType);
	
	// If random selected, pick a random type
	int type = 0;
	if (InTerrainType == 5) { type = FMath::RandRange(0, 4); }
	else { type = InTerrainType; }

	// Initialise the terrain with UI data
	LandActor->Init(seed, type);
}

void ATeamProjectGameModeBase::OnStart()
{
	if (PlayerCharacterClass)
	{
		FTransform Transform;
		FVector Location = { 0,0,1000 };
		Transform.SetTranslation(Location);

		APlayerCharacter* PlayerActor = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, Transform);
		PlayerController->UnPossess();
		PlayerController->AutoReceiveInput = EAutoReceiveInput::Player0;
		PlayerController->Possess((PlayerActor));

		PlayerPawn->Destroy();
		PlayerPawn = PlayerActor;

		PlayerController->RebindCharacter(PlayerActor);
	}
}

void ATeamProjectGameModeBase::OnBack()
{
	LandActor->Clear();
}
