// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MainPlayerController.h"
#include "Land.h"
#include "BackLand.h"

#include "PlayerCharacter.h"
#include "WaveManager.h"
#include "Engine/SceneCapture2D.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TeamProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ATeamProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATeamProjectGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly)
	bool GameStart = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ALand> LandClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABackLand> BackLandClass;

	//Reference to the PlayerCharacters Class
	UPROPERTY(EditAnywhere)
		TSubclassOf<APlayerCharacter> PlayerCharacterClass;

	//Reference to the WaveManager
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWaveManager> WaveManagerClass;

	//Reference to the PlayerController
	UPROPERTY()
	AMainPlayerController* PlayerController;

	//Reference to the Player Pawn
	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY()
	ALand* LandActor;

	UPROPERTY()
	TArray<ABackLand*> BackLandActors;

	UPROPERTY(BlueprintReadWrite)
		AWaveManager* WaveManager;

	//Update the Land based on the values the Player sets in Preview Panel
	UFUNCTION(BlueprintCallable)
		void OnGuiSetValues(FText InSeedString, int InTerrainType);

	//Spawn and Possess the Player
	UFUNCTION(BlueprintCallable)
		void OnStart();

	UFUNCTION(BlueprintCallable)
		void OnBack();

	UFUNCTION(BlueprintCallable)
		void OnPlay();

	//MiniMap and Preview
	UPROPERTY(EditAnywhere)
		TSubclassOf<ASceneCapture2D> PreviewCaptureClass;

	UPROPERTY(EditAnywhere)
		UTextureRenderTarget2D* PreviewTextureTarget;
	
	UPROPERTY()
		ASceneCapture2D* PreviewCapture;

private:
	UPROPERTY(EditAnywhere, Category = Audio)
		UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = Audio)
		TMap<FString, USoundBase*> BackgroundSoundMap;
	
	UPROPERTY(EditAnywhere)
		bool EnableWaveManager = true;

	UPROPERTY()
	 bool IsWaveStarted = false;

	APlayerCharacter* PlayerActor;

};
