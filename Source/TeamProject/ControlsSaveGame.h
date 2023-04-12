// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnhancedActionKeyMapping.h"
#include "Rendering/RenderingCommon.h"
#include "GameFramework/SaveGame.h"
#include "ControlsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UControlsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<FEnhancedActionKeyMapping> PlayerKeyMappings;

	UPROPERTY()
		float VolumeModifier = 0.0f;

	UPROPERTY()
		float PlayerMouseSensitivity = 0.5f;

	UPROPERTY()
		float PlayerInvertedMouseX = 1.0f;
	
	UPROPERTY()
		float PlayerInvertedMouseY = 1.0f;

	UPROPERTY()
		EColorVisionDeficiency PlayerColourVisionDeficiency = EColorVisionDeficiency::NormalVision;

	UPROPERTY()
		float PlayerColourVisionDeficiencySeverity = 1.0f;
	
};
