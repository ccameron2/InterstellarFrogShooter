// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"


AMainPlayerController::AMainPlayerController()
{

}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<APlayerCharacter>(GetPawn());
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis(TEXT("Move Forward"), this, &AMainPlayerController::CallMoveForwards);
	InputComponent->BindAxis(TEXT("Strafe"), this, &AMainPlayerController::CallStrafe);
	InputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayerController::CallTurn);
	InputComponent->BindAxis(TEXT("Look Up"), this, &AMainPlayerController::CallLookUp);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayerController::CallJump);
}

void AMainPlayerController::CallMoveForwards(float AxisAmount)
{
	if (Character)
	{
		Character->MoveForwards(AxisAmount);
	}
}

//Allows the player to move the character left and right
void AMainPlayerController::CallStrafe(float AxisAmount)
{
	if (Character)
	{
		Character->Strafe(AxisAmount);
	}
}

//Allows the player to rotate the character up and down
void AMainPlayerController::CallLookUp(float AxisAmount)
{
	if (Character)
	{
		Character->LookUp(AxisAmount);
	}
}

//Allows the player to rotate the character left and right
void AMainPlayerController::CallTurn(float AxisAmount)
{
	if (Character)
	{
		Character->Turn(AxisAmount);
	}
}

void AMainPlayerController::CallJump()
{
	if (Character)
	{
		Character->Jump();
	}
}

