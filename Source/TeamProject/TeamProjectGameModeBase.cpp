// CCameron, JMills
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

	//Spawn in a SceneCapture2D component to capture the World for the Preview Panel
	PreviewCapture = GetWorld()->SpawnActor<ASceneCapture2D>(PreviewCaptureClass);
	PreviewCapture->GetCaptureComponent2D()->TextureTarget = PreviewTextureTarget;
	
	PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerPawn = PlayerController->GetPawn();

	FTransform Transform;
	FVector Location = { 0,0,0 };
	Transform.SetTranslation(Location);

	// Create noise object, set noise type to fractal and set seed.
	FastNoise noise;
	noise.SetNoiseType(FastNoise::SimplexFractal);
	noise.SetSeed(FMath::RandRange(0, 999));

	// Spawn the main arena
	int Type = 0;
	if (LandClass)
	{
		LandActor = GetWorld()->SpawnActor<ALand>(LandClass, Transform);
		Type = FMath::RandRange(0, LandActor->NumTypes - 1);
		LandActor->Init(Type, &noise);
	}

	auto size = LandActor->Size * LandActor->Scale;
	auto scale = LandActor->Scale;

	// Spawn background terrain
	if (BackLandClass)
	{
		for (int i = 0; i < 8; i++)
		{
			// Set location for each background terrain block
			if(i == 0) Location = FVector{-size + scale,size - scale ,0};
			if(i == 1) Location = FVector{0,size - scale,0};
			if(i == 2) Location = FVector{size - scale,size - scale,0};
			if(i == 3) Location = FVector{-size + scale,0,0};
			if(i == 4) Location = FVector{size - scale,0,0};
			if(i == 5) Location = FVector{-size + scale,-size + scale,0};
			if(i == 6) Location = FVector{0,-size + scale,0};
			if(i == 7) Location = FVector{size - scale,-size + scale,0};
			Transform.SetLocation(Location);

			auto backLandActor = GetWorld()->SpawnActor<ABackLand>(BackLandClass, Transform);
			backLandActor->Init(Type, &noise);
			BackLandActors.Push(backLandActor);
		}
	}

	Location = { 0,0,1000 };
	Transform.SetTranslation(Location);

	if(EnableWaveManager)
	{
		// Spawn a wave manager to spawn frogs
		WaveManager = GetWorld()->SpawnActor<AWaveManager>(WaveManagerClass, Transform);
		WaveManager->Init(LandActor->Size * LandActor->Scale);

		// Spawn title background frogs
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

	if (PlayerActor)
	{
		if (LandActor->TerrainType != Desert && LandActor->TerrainType != Snowy)
		{
			if (PlayerActor->GetActorLocation().Z < LandActor->WaterLevel)
			{
				PlayerActor->Underwater = true;
			}
			else
			{
				PlayerActor->Underwater = false;
			}
		}
		
	}
}

//Update the Land based on the values the Player sets in Preview Panel
void ATeamProjectGameModeBase::OnGuiSetValues(FText InSeedString, int InTerrainType)
{
	// Clear title screen frogs
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

//Spawn and Possess the Player
void ATeamProjectGameModeBase::OnStart()
{
	GameStart = true;
	if (PlayerCharacterClass)
	{
		// Spawn character into the world
		FTransform Transform;
		const FVector Location = { 0,0,1000 };
		Transform.SetTranslation(Location);

		PlayerActor = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, Transform);

		// Possess with custom player controller
		PlayerController->UnPossess();
		PlayerController->AutoReceiveInput = EAutoReceiveInput::Player0;
		PlayerController->Possess((PlayerActor));

		// Destroy old actor
		PlayerPawn->Destroy();
		PlayerPawn = PlayerActor;
		PlayerController->RebindCharacter(PlayerActor);
	}
	
	if(EnableWaveManager)
	{
		// Clear title frogs and start wave timer
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