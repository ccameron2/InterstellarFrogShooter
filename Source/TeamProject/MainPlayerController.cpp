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

	//Get the EnhancedInput subsystem
	//if it exists create a new Mapping Context with all the required Input Actions
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	//Check whether a SaveGame exists in the slot
	//Otherwise create a new SaveGame in the slot
	if(UGameplayStatics::DoesSaveGameExist(ControlSaveGameName, 0))
	{
		//Load in the SaveGame and get all the saved values
		ControlSaveGame = Cast<UControlsSaveGame>(UGameplayStatics::LoadGameFromSlot(ControlSaveGameName, 0));
		KeyMappings = ControlSaveGame->PlayerKeyMappings;
		MouseSensitivity = ControlSaveGame->PlayerMouseSensitivity;
		InvertMouseXValue = ControlSaveGame->PlayerInvertedMouseX;
		InvertMouseYValue = ControlSaveGame->PlayerInvertedMouseY;
		ColourVisionDeficiency = ControlSaveGame->PlayerColourVisionDeficiency;
		ColourVisionDeficiencySeverity = ControlSaveGame->PlayerColourVisionDeficiencySeverity;
		PlayerHighScore = ControlSaveGame->PlayerHighScore;
	}
	else
	{
		CreateSaveGame();
	}

	//Create all the UserWidgets
	Menu = CreateWidget(this, MainMenuWidget);
	HUD = CreateWidget(this, HUDWidget);
	Settings = CreateWidget(this, SettingsWidget);
	PauseWidget = CreateWidget(this, PauseUserWidget);
	CreditsWidget = CreateWidget(this, CreditsUserWidgets);
	
	//Add the MainMenu UserWidget to the screen
	//and set the Players input to be in the Game
	Menu->AddToViewport();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	//Get a reference to the PlayerControllers' Pawn
	Character = Cast<APlayerCharacter>(GetPawn());
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	
	//Get the EnhancedInputComponent and Bind keyActions to Different Functions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &AMainPlayerController::PauseGame);
		EnhancedInputComponent->BindAction(SkillTreeAction, ETriggerEvent::Completed, this, &AMainPlayerController::AddOrRemoveSkillTree);
		
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallTurn);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallLookUp);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallJump);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMainPlayerController::CallStopFire);

		EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Completed, this, &AMainPlayerController::CallChangeWeapon);
		
		EnhancedInputComponent->BindAction(StrafeAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallStrafe);
		EnhancedInputComponent->BindAction(ForwardAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallMoveForwards);
	}
}

void AMainPlayerController::CallMoveForwards(const FInputActionValue& Value)
{
	const float ForwardValue = Value.Get<float>();
	if (Character)
	{
		Character->MoveForwards(ForwardValue);
	}
}

//Allows the Player to move the character left and right
void AMainPlayerController::CallStrafe(const FInputActionValue& Value)
{
	const float StrafeValue = Value.Get<float>();
	if (Character)
	{
		Character->Strafe(StrafeValue);
	}
}

//Allows the Player to rotate the character up and down
void AMainPlayerController::CallLookUp(const FInputActionValue& Value)
{
	const float AxisAmount = Value.Get<float>();
	float RotationAmount = AxisAmount * InvertMouseYValue * MouseSensitivity;
	if (Character)
	{
		Character->LookUp(RotationAmount);
	}
}

//Allows the Player to rotate the character left and right
void AMainPlayerController::CallTurn(const FInputActionValue& Value)
{
	const float AxisAmount = Value.Get<float>();
	
	float RotationAmount = AxisAmount * InvertMouseXValue * MouseSensitivity;
	if (Character)
	{
		Character->Turn(RotationAmount);
	}
}

//Allows the Player to jump
void AMainPlayerController::CallJump()
{
	if (Character)
	{
		Character->Jump();
	}
}

//When starting the Game rebind the PlayerCharacter reference to the newly spawned in Player 
void AMainPlayerController::RebindCharacter(APlayerCharacter* playerCharacter)
{
	Character = nullptr;
	Character = Cast<APlayerCharacter>(playerCharacter);
}

//Start firing the weapon on the PlayerCharacter
void AMainPlayerController::CallFire()
{
	if(Character)
	{
		Character->StartFireWeapon();
	}
}

//Stop firing the weapon on the PlayerCharacter
void AMainPlayerController::CallStopFire()
{
	if(Character)
	{
		Character->StopFireWeapon();
	}
}

//Swap the currently selected weapon on the PlayerCharacter
void AMainPlayerController::CallChangeWeapon()
{
	if(Character)
	{
		Character->ChangeWeapon();
	}
}

//Switch between different User Widgets
void AMainPlayerController::WidgetLoader(int index)
{
	if (index == 0) //MainMenu
	{
		Menu->AddToViewport();

		//Remove all other UserWidgets
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 1) //HUD
	{
		//Resume the game and add the HUD to the screen
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->AddToViewport();

		//Remove all other UserWidgets
		Menu->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 2) //Settings
	{
		Settings->AddToViewport();

		//Remove all other UserWidgets
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 3) //EndScreen
	{
		//Create the EndScreen and add it to the screen
		EndScreen = CreateWidget(this, EndScreenWidget);
		EndScreen->AddToViewport();

		//Remove all other UserWidgets
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();

		//Set the Players Input to UI only
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
		
	}
	else if (index == 4) //Pause
	{
		//only add the Pause Menu if the Skill is currently closed
	    if(!SkillTree->IsInViewport())
	    {
	    	PauseWidget->AddToViewport();
			Menu->RemoveFromParent();
			HUD->RemoveFromParent();
			Settings->RemoveFromParent();
			CreditsWidget->RemoveFromParent();
	    }
	}

	else if (index == 5) //Credits
	{
		CreditsWidget->AddToViewport();

		//Remove all other UserWidgets
		PauseWidget->RemoveFromParent();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
	}
}

//Pause the Game and update the Players Input Mode respectively 
void AMainPlayerController::PauseGame()
{
	if(UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		WidgetLoader(1);
	}
	else
	{
		WidgetLoader(4);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

//Update whether the Skill Tree is being shown on screen 
void AMainPlayerController::AddOrRemoveSkillTree()
{
	//The Skill Tree exists
	if(SkillTree)
	{
		//The Skill Tree is currently on the screen
		if(SkillTree->IsInViewport())
		{
			SkillTree->RemoveFromParent();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
		else //The Skill Tree is currently closed
		{
			SkillTree->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
		}
	}
}

//Save the game to the SaveGame slot
void AMainPlayerController::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(ControlSaveGame, ControlSaveGameName, 0);
}

//Create a new SaveGame if none exist
UControlsSaveGame* AMainPlayerController::CreateSaveGame()
{
	ControlSaveGame = Cast<UControlsSaveGame>(UGameplayStatics::CreateSaveGameObject(ControlSaveGameSubclass));
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::SetKeyMappings, 0.5f, false);
	return ControlSaveGame;
}

//Update the Subsystems keyMappings with a new Key set by the player
void AMainPlayerController::UpdateMapping(FText DisplayName, FKey Key)
{
	Subsystem->AddPlayerMappedKey(*DisplayName.ToString(), Key);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::DelayUpdatingKeyMappings, 0.1f, false);
}

//Update the SaveGames' KeyMappings
void AMainPlayerController::UpdateKeyMappings()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerKeyMappings = KeyMappings;

	SaveGame();
}

//Update the SaveGame's Mouse sensitivity level
void AMainPlayerController::UpdateSaveGameMouseSensitivity()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerMouseSensitivity = MouseSensitivity;

	SaveGame();
}

//Update the SaveGame's Mouse inverted X value
void AMainPlayerController::UpdateSaveGameInvertMouseX()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerInvertedMouseX = InvertMouseXValue;

	SaveGame();
}

//Update the SaveGame's Mouse inverted Y value
void AMainPlayerController::UpdateSaveGameInvertMouseY()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerInvertedMouseY = InvertMouseYValue;

	SaveGame();
}

//Update the SaveGame's Colour Deficiency type
void AMainPlayerController::UpdateSaveGameColourDeficiency()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerColourVisionDeficiency = ColourVisionDeficiency;

	SaveGame();
}

//Update the SaveGame's Colour Deficiency severity
void AMainPlayerController::UpdateSaveGameColourDeficiencySeverity()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerColourVisionDeficiencySeverity = ColourVisionDeficiencySeverity;

	SaveGame();
}

//Update the SaveGame's High score
void AMainPlayerController::UpdateSaveGamePlayerHighScore(bool bReset)
{
	if(ControlSaveGame)
	{
		if(bReset) //Reset the Players current and High score
		{
			PlayerHighScore = 0;
			ControlSaveGame->PlayerHighScore = 0;
		}
		else //check if the Player has beat their High score
		{
			if(PlayerScore > PlayerHighScore)
			{
				ControlSaveGame->PlayerHighScore = PlayerScore;
				PlayerHighScore = PlayerScore;
			}
		}
	}

	SaveGame();
}

//Set keyMappings to the SubSystems PlayerMappableKeyMappings
//Delayed as it takes the Subsystem a Tick or more to update
void AMainPlayerController::DelayUpdatingKeyMappings()
{
	KeyMappings = Subsystem->GetAllPlayerMappableActionKeyMappings();
}

//Get the Players keyBindings
//called after a short delay as this is the only way to get the Players' keyBindings in Unreal

void AMainPlayerController::SetKeyMappings()
{
	KeyMappings = Subsystem->GetAllPlayerMappableActionKeyMappings();

	ControlSaveGame->PlayerKeyMappings = KeyMappings;

	SaveGame();
}