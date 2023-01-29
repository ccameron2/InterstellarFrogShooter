// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MainPlayerController.h"
#include "Land.h"
#include <string>
#include "PlayerCharacter.h"
#include "WaveManager.h"

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

private:
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ALand> LandClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APlayerCharacter> PlayerCharacterClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AWaveManager> WaveManagerClass;

	AMainPlayerController* PlayerController;
	APawn* PlayerPawn;
	ALand* LandActor;
	AWaveManager* WaveManager;

	UFUNCTION(BlueprintCallable)
		void OnGuiSetValues(FText InSeedString, int InTerrainType);

	UFUNCTION(BlueprintCallable)
		void OnStart();

	UFUNCTION(BlueprintCallable)
		void OnBack();

	UFUNCTION(BlueprintCallable)
		void OnPlay();

	UFUNCTION(BlueprintCallable)
		void OnDebug();
};
