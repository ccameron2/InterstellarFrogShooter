// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


AMainPlayerController::AMainPlayerController()
{
	 
	
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	Menu = CreateWidget(this, MainMenuWidget);
	HUD = CreateWidget(this, HUDWidget);
	Settings = CreateWidget(this, SettingsWidget);
	//SkillTree = CreateWidget(this, SkillTreeWidget);
	PauseWidget = CreateWidget(this, PauseUserWidget);
	CreditsWidget = CreateWidget(this, CreditsUserWidgets);

	Menu->AddToViewport();
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

	Character = Cast<APlayerCharacter>(GetPawn());
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis(TEXT("Move Forward"), this, &AMainPlayerController::CallMoveForwards);
	InputComponent->BindAxis(TEXT("Strafe"), this, &AMainPlayerController::CallStrafe);
	InputComponent->BindAxis(TEXT("Turn"), this, &AMainPlayerController::CallTurn);
	InputComponent->BindAxis(TEXT("Look up"), this, &AMainPlayerController::CallLookUp);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayerController::CallJump);
	InputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AMainPlayerController::PauseGame);
	InputComponent->BindAction(TEXT("CheatsMode"), IE_Pressed, this, &AMainPlayerController::UpdateDeveloperMode);

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
	float RotationAmount = AxisAmount * InvertMouseYValue * MouseSensitivity;
	UE_LOG(LogTemp, Warning, TEXT("Rotation Amount: %f, InvertMouseYSense: %f"), RotationAmount, InvertMouseYValue)
	if (Character)
	{
		Character->LookUp(RotationAmount);
	}
}

//Allows the player to rotate the character left and right
void AMainPlayerController::CallTurn(float AxisAmount)
{
	float RotationAmount = AxisAmount * InvertMouseXValue * MouseSensitivity;
	UE_LOG(LogTemp, Warning, TEXT("Rotation Amount: %f, InvertMouseXSense: %f"), RotationAmount, InvertMouseXValue)
	if (Character)
	{
		Character->Turn(RotationAmount);
	}
}

void AMainPlayerController::CallJump()
{
	if (Character)
	{
		Character->Jump();
	}
}

void AMainPlayerController::RebindCharacter(APlayerCharacter* playerCharacter)
{
	Character = nullptr;
	Character = Cast<APlayerCharacter>(playerCharacter);
}

void AMainPlayerController::WidgetLoader(int index)
{
	if (index == 0)
	{
		Menu->AddToViewport();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		//SkillTree->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 1)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->AddToViewport();
		Menu->RemoveFromParent();
		Settings->RemoveFromParent();
		//SkillTree->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 2)
	{
		Settings->AddToViewport();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		//SkillTree->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 3)
	{
		// SkillTree->AddToViewport();
		// Menu->RemoveFromParent();
		// HUD->RemoveFromParent();
		// Settings->RemoveFromParent();
		// PauseWidget->RemoveFromParent();
		// FInputModeGameAndUI temp = FInputModeGameAndUI();
		// temp.SetHideCursorDuringCapture(false);
		// SetInputMode(temp);
		// SetShowMouseCursor(true);
		// CreditsWidget->RemoveFromParent();
		
	}
	else if (index == 4)
	{
		PauseWidget->AddToViewport();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		//SkillTree->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}

	else if (index == 5)
	{
		CreditsWidget->AddToViewport();
		PauseWidget->RemoveFromParent();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		//SkillTree->RemoveFromParent();
	}
}

void AMainPlayerController::PauseGame()
{
	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		WidgetLoader(1);
	}
	else
	{
		WidgetLoader(4);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void AMainPlayerController::UpdateDeveloperMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Updated Developer Mode"));
	bDeveloperMode = !bDeveloperMode;
}
