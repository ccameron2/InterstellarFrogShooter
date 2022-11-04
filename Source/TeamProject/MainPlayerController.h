// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "DroneCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AMainPlayerController();

	virtual void SetupInputComponent();

	void CallMoveForwards(float AxisAmount);

	void CallStrafe(float AxisAmount);

	void CallLookUp(float AxisAmount);

	void CallTurn(float AxisAmount);

	void CallJump();

	APlayerCharacter* Character;

	ADroneCharacter* Drone;
	
};
