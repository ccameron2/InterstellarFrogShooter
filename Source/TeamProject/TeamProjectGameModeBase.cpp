// Copyright Epic Games, Inc. All Rights Reserved.
#include "TeamProjectGameModeBase.h"

#include "Components/AudioComponent.h"
#include "Components/SceneCaptureComponent2D.h"


ATeamProjectGameModeBase::ATeamProjectGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("BackgroundMusic");
	AudioComponent->SetAutoActivate(false);
}

void ATeamProjectGameModeBase::BeginPlay()
{
	if(BackgroundSoundMap.Contains("Menu"))
	{
		AudioComponent->SetSound(*BackgroundSoundMap.Find("Menu"));
		AudioComponent->Play();
	}
		
	MiniMapCapture = GetWorld()->SpawnActor<ASceneCapture2D>(MiniMapCaptureClass);

	PreviewCapture = GetWorld()->SpawnActor<ASceneCapture2D>(MiniMapCaptureClass);
	PreviewCapture->GetCaptureComponent2D()->TextureTarget = PreviewTextureTarget;
	
	PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerPawn = PlayerController->GetPawn();

	FTransform transform;
	FVector Location = { 0,0,0 };
	transform.SetTranslation(Location);

	// Create noise object, set noise type to fractal and set seed.
	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetSeed(FMath::RandRange(0, 999));

	int type = 0;
	if (LandClass)
	{
		LandActor = GetWorld()->SpawnActor<ALand>(LandClass, transform);
		type = FMath::RandRange(0, LandActor->NumTypes - 1);
		LandActor->Init(type, &noise);
	}

	auto location = FVector{ 0 };
	auto size = LandActor->Size * LandActor->Scale;
	auto scale = LandActor->Scale;

	if (BackLandClass)
	{
		for (int i = 0; i < 8; i++)
		{
			if(i == 0) location = FVector{-size + scale,size - scale ,0};
			if(i == 1) location = FVector{0,size - scale,0};
			if(i == 2) location = FVector{size - scale,size - scale,0};
			if(i == 3) location = FVector{-size + scale,0,0};
			if(i == 4) location = FVector{size - scale,0,0};
			if(i == 5) location = FVector{-size + scale,-size + scale,0};
			if(i == 6) location = FVector{0,-size + scale,0};
			if(i == 7) location = FVector{size - scale,-size + scale,0};
			transform.SetLocation(location);

			auto backLandActor = GetWorld()->SpawnActor<ABackLand>(BackLandClass, transform);
			backLandActor->Init(type, &noise);
			BackLandActors.Push(backLandActor);
		}
	}

	Location = { 0,0,1000 };
	transform.SetTranslation(Location);

	if(EnableWaveManager)
	{
		WaveManager = GetWorld()->SpawnActor<AWaveManager>(WaveManagerClass, transform);
		WaveManager->Init(LandActor->Size * LandActor->Scale);
		WaveManager->SpawnTitleFrogs();
	}
}

void ATeamProjectGameModeBase::Tick(float DeltaTime)
{
	if (GameStart)
	{
		if(BackgroundSoundMap.Contains("Ambient"))
		{
			AudioComponent->SetSound(*BackgroundSoundMap.Find("Ambient"));
		}

		GameStart = false;
	}

	if(EnableWaveManager && !IsWaveStarted)
	{
		if(WaveManager->WaveNum == 1)
		{
			if(BackgroundSoundMap.Contains("Wave"))
			{
				AudioComponent->SetSound(*BackgroundSoundMap.Find("Wave"));
			}
			IsWaveStarted = true;
		}
	}
}

void ATeamProjectGameModeBase::OnGuiSetValues(FText InSeedString, int InTerrainType)
{
	if(EnableWaveManager)
		WaveManager->ClearFrogs();

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
	if (InTerrainType == LandActor->NumTypes) { type = FMath::RandRange(0, LandActor->NumTypes - 1); }
	else { type = InTerrainType; }

	// Create noise object, set noise type to fractal and set seed.
	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetSeed(seed);

	// Initialise the terrain with UI data
	LandActor->Init(type, &noise);

	for (auto& backLand : BackLandActors)
	{
		backLand->Init(type, &noise);
	}

	// Force rebuild Nav Mesh
	LandActor->SetActorLocation(FVector{0, 1, 0});
	LandActor->SetActorLocation(FVector{0, 0, 0});
}

void ATeamProjectGameModeBase::OnStart()
{
	GameStart = true;
	if (PlayerCharacterClass)
	{
		FTransform Transform;
		FVector Location = { 0,0,1000 };
		Transform.SetTranslation(Location);

		APlayerCharacter* PlayerActor = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, Transform);
		PlayerController->UnPossess();
		PlayerController->AutoReceiveInput = EAutoReceiveInput::Player0;
		PlayerController->Possess((PlayerActor));
		PlayerActor->UpdateDeveloperMode(PlayerController->bDeveloperMode);

		PlayerPawn->Destroy();
		PlayerPawn = PlayerActor;

		PlayerController->RebindCharacter(PlayerActor);

		MiniMapCapture->AttachToActor(PlayerActor, FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	if(EnableWaveManager)
	{
		WaveManager->ClearFrogs();
		WaveManager->StartWaves();;
	}
}

void ATeamProjectGameModeBase::OnBack()
{
	if(EnableWaveManager)
		WaveManager->SpawnTitleFrogs();
}

void ATeamProjectGameModeBase::OnPlay()
{
	if(EnableWaveManager)
		WaveManager->ClearFrogs();
}

void ATeamProjectGameModeBase::OnDebug()
{
	if(EnableWaveManager)
		WaveManager->ClearFrogs();
	
	LandActor->Clear();
}
