//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnhancedActionKeyMapping.h"
#include "Rendering/RenderingCommon.h"
#include "GameFramework/SaveGame.h"
#include "ControlsSaveGame.generated.h"

/**
 *	All Variables in this class get saved to an Unreal Engine SaveGame file than is read in when the game starts
 */
UCLASS()
class TEAMPROJECT_API UControlsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	//Reference to the Players KeyBindings
	UPROPERTY()
		TArray<FEnhancedActionKeyMapping> PlayerKeyMappings;

	//Reference to the Players Volume level
	UPROPERTY()
		float VolumeModifier = 0.0f;

	//Reference to the Players Mouse sensitivity level
	UPROPERTY()
		float PlayerMouseSensitivity = 0.5f;

	//Reference to whether the Player has inverted their Mouse in the X Axis
	UPROPERTY()
		float PlayerInvertedMouseX = 1.0f;

	//Reference to whether the Player has inverted their Mouse in the Y Axis
	UPROPERTY()
		float PlayerInvertedMouseY = 1.0f;

	//Reference to the Players Colour Deficiency Type
	UPROPERTY()
		EColorVisionDeficiency PlayerColourVisionDeficiency = EColorVisionDeficiency::NormalVision;

	//Reference to the Players Colour Deficiency Severity level
	UPROPERTY()
		float PlayerColourVisionDeficiencySeverity = 1.0f;

	//Reference to the Players HighScore
	UPROPERTY()
		float PlayerHighScore = 0.0f;	
};