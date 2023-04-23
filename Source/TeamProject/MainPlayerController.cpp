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

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if(UGameplayStatics::DoesSaveGameExist(ControlSaveGameName, 0))
	{
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
	
	Menu = CreateWidget(this, MainMenuWidget);
	HUD = CreateWidget(this, HUDWidget);
	Settings = CreateWidget(this, SettingsWidget);

	PauseWidget = CreateWidget(this, PauseUserWidget);
	CreditsWidget = CreateWidget(this, CreditsUserWidgets);
	

	Menu->AddToViewport();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

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

//Allows the player to move the character left and right
void AMainPlayerController::CallStrafe(const FInputActionValue& Value)
{
	const float StrafeValue = Value.Get<float>();
	if (Character)
	{
		Character->Strafe(StrafeValue);
	}
}

//Allows the player to rotate the character up and down
void AMainPlayerController::CallLookUp(const FInputActionValue& Value)
{
	const float AxisAmount = Value.Get<float>();
	float RotationAmount = AxisAmount * InvertMouseYValue * MouseSensitivity;
	if (Character)
	{
		Character->LookUp(RotationAmount);
	}
}

//Allows the player to rotate the character left and right
void AMainPlayerController::CallTurn(const FInputActionValue& Value)
{
	const float AxisAmount = Value.Get<float>();
	
	float RotationAmount = AxisAmount * InvertMouseXValue * MouseSensitivity;
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

void AMainPlayerController::CallFire()
{
	if(Character)
	{
		Character->StartFireWeapon();
	}
}

void AMainPlayerController::CallStopFire()
{
	if(Character)
	{
		Character->StopFireWeapon();
	}
}

void AMainPlayerController::CallChangeWeapon()
{
	if(Character)
	{
		Character->ChangeWeapon();
	}
}

void AMainPlayerController::WidgetLoader(int index)
{
	if (index == 0)
	{
		Menu->AddToViewport();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 1)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUD->AddToViewport();
		Menu->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 2)
	{
		Settings->AddToViewport();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
	}
	else if (index == 3)
	{
		EndScreen = CreateWidget(this, EndScreenWidget);
		EndScreen->AddToViewport();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
		PauseWidget->RemoveFromParent();
		CreditsWidget->RemoveFromParent();
		
		const FInputModeUIOnly Temp = FInputModeUIOnly();
		SetInputMode(Temp);
		SetShowMouseCursor(true);
		
	}
	else if (index == 4)
	{
	    if(!SkillTree->IsInViewport())
	    {
	    	PauseWidget->AddToViewport();
			Menu->RemoveFromParent();
			HUD->RemoveFromParent();
			Settings->RemoveFromParent();
			CreditsWidget->RemoveFromParent();
	    }
	}

	else if (index == 5)
	{
		CreditsWidget->AddToViewport();
		PauseWidget->RemoveFromParent();
		Menu->RemoveFromParent();
		HUD->RemoveFromParent();
		Settings->RemoveFromParent();
	}
}

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

void AMainPlayerController::AddOrRemoveSkillTree()
{
	if(SkillTree)
	{
		if(SkillTree->IsInViewport())
		{
			SkillTree->RemoveFromParent();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
		else
		{
			SkillTree->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
		}
	}
}

void AMainPlayerController::UpdateDeveloperMode()
{
	bDeveloperMode = !bDeveloperMode;
}

void AMainPlayerController::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(ControlSaveGame, ControlSaveGameName, 0);
}

UControlsSaveGame* AMainPlayerController::CreateSaveGame()
{
	ControlSaveGame = Cast<UControlsSaveGame>(UGameplayStatics::CreateSaveGameObject(ControlSaveGameSubclass));

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::SetKeyMappings, 0.5f, false);
	return ControlSaveGame;
}

void AMainPlayerController::UpdateMapping(FText DisplayName, FKey Key)
{
	Subsystem->AddPlayerMappedKey(*DisplayName.ToString(), Key);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainPlayerController::DelayUpdatingKeyMappings, 0.1f, false);
}

void AMainPlayerController::UpdateKeyMappings()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerKeyMappings = KeyMappings;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGameMouseSensitivity()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerMouseSensitivity = MouseSensitivity;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGameInvertMouseX()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerInvertedMouseX = InvertMouseXValue;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGameInvertMouseY()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerInvertedMouseY = InvertMouseYValue;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGameColourDeficiency()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerColourVisionDeficiency = ColourVisionDeficiency;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGameColourDeficiencySeverity()
{
	if(ControlSaveGame)
		ControlSaveGame->PlayerColourVisionDeficiencySeverity = ColourVisionDeficiencySeverity;

	SaveGame();
}

void AMainPlayerController::UpdateSaveGamePlayerHighScore(bool bReset)
{
	if(ControlSaveGame)
	{
		if(bReset)
		{
			PlayerHighScore = 0;
			ControlSaveGame->PlayerHighScore = 0;
		}
		else
		{
			if(PlayerScore > PlayerHighScore)
				ControlSaveGame->PlayerHighScore = PlayerScore;
			PlayerHighScore = PlayerScore;
		}
	}

	SaveGame();
}

void AMainPlayerController::DelayUpdatingKeyMappings()
{
	KeyMappings = Subsystem->GetAllPlayerMappableActionKeyMappings();
}

void AMainPlayerController::SetKeyMappings()
{
	KeyMappings = Subsystem->GetAllPlayerMappableActionKeyMappings();

	ControlSaveGame->PlayerKeyMappings = KeyMappings;

	SaveGame();
}
