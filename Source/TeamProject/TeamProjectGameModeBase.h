// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MainPlayerController.h"
#include "Land.h"
#include <string>
#include "PlayerCharacter.h"
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

	AMainPlayerController* PlayerController;
	APawn* PlayerPawn;
	ALand* LandActor;

	UFUNCTION(BlueprintCallable)
		void OnGuiSetValues(FText InSeedString, int InTerrainType);

	UFUNCTION(BlueprintCallable)
		void OnStart();

};
