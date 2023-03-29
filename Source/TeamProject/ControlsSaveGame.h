// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnhancedActionKeyMapping.h"

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
		float PlayerInvertedMouseX;
	
	UPROPERTY()
		float PlayerInvertedMouseY;
	
	
};
