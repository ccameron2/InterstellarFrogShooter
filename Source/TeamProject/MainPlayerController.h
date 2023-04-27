// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ControlsSaveGame.h"
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

	//Switch between different User Widgets
	UFUNCTION(BlueprintCallable)
		void WidgetLoader(int index);

	//Pause the Game and update the Players Input Mode respectively 
	void PauseGame();

	//Update whether the Skill Tree is being shown on screen 
	void AddOrRemoveSkillTree();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInGame = false;
	
	//Reference to the Player Character
	APlayerCharacter* Character;

	//--------------------------//
	//		Input System		//
	//--------------------------//

	virtual void SetupInputComponent();

	void CallMoveForwards(const FInputActionValue& Value);

	void CallStrafe(const FInputActionValue& Value);

	void CallLookUp(const FInputActionValue& Value);

	void CallTurn(const FInputActionValue& Value);

	void CallJump();

	void RebindCharacter(APlayerCharacter* playerCharacter);

	void CallFire();

	void CallStopFire();

	void CallChangeWeapon();

	//The Mapping Context where we set up all the Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
		UInputMappingContext* DefaultMappingContext;

	//The Players KeyBindings
	UPROPERTY(BlueprintReadOnly)
		TArray<FEnhancedActionKeyMapping> KeyMappings;

	//The Input Subsystem needed for EnhancedInput
	UPROPERTY()
		UEnhancedInputLocalPlayerSubsystem* Subsystem;

	//----------------------//
	//	Input Actions		//
	//----------------------//

	//Input Action for Pausing the Game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
		UInputAction* PauseAction;

	//Input Action for turning the Player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
		UInputAction* TurnAction;
	
	//Input Action for rotating the Player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
		UInputAction* LookUpAction;
	
	//Input Action for changing weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* ChangeWeaponAction;

	//Input Action for Firing the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* FireAction;

	//Input Action for Jumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	//Input Action for Moving Left and Right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* StrafeAction;

	//Input Action for Moving Forwards and Backwards
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* ForwardAction;

	//Input Action for displaying the Skill Tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* SkillTreeAction;

	//--------------------------//
	//		Mouse Settings		//
	//--------------------------//

	//Whether the Mouse is inverted in the X Axis
	//Float because the way UnrealEngine deals with Inverted Mouse is by multiplying by -1
	UPROPERTY(BlueprintReadWrite)
		float InvertMouseXValue = 1.0f;

	//Whether the Mouse is inverted in the Y Axis
	//Float because the way UnrealEngine deals with Inverted Mouse is by multiplying by -1
	UPROPERTY(BlueprintReadWrite)
		float InvertMouseYValue = 1.0f;

	//Mouse sensitivity level
	UPROPERTY(BlueprintReadWrite)
		float MouseSensitivity = 1.0f;

	//Invert the Players Mouse in the X Axis
	UFUNCTION(BlueprintCallable)
		void InvertMouseX() {InvertMouseXValue *= -1;} 

	//Invert the Players Mouse in the X Axis
	UFUNCTION(BlueprintCallable)
		void InvertMouseY() {InvertMouseYValue *= -1;}

	//Reference to the SkillTree User Widget
	UPROPERTY(BlueprintReadWrite)
		UUserWidget* SkillTree;

	//--------------------------//
	//		Accessibility 		//
	//--------------------------//

	//The Players Colour Deficiency type
	UPROPERTY(BlueprintReadWrite)
		EColorVisionDeficiency ColourVisionDeficiency = EColorVisionDeficiency::NormalVision;

	//The Players Colour Deficiency severity
	UPROPERTY(BlueprintReadWrite)
		float ColourVisionDeficiencySeverity = 1.0f;

	//--------------------------//
	//		Score       		//
	//--------------------------//
	
	//The Players High score
	UPROPERTY(BlueprintReadWrite)
		float PlayerHighScore = 0.0f;

	//The Players current score
	UPROPERTY(BlueprintReadWrite)
		float PlayerScore = 0.0f;

	UFUNCTION(BlueprintCallable)
		void SetPlayerScore(const float Amount) {PlayerScore += Amount;}
	
	//--------------------------//
	//		SaveGame     		//
	//--------------------------//

	//Save the game to the SaveGame slot
	UFUNCTION(BlueprintCallable)
		void SaveGame();

	//Create a new SaveGame if none exist
	UFUNCTION(BlueprintCallable)
		UControlsSaveGame* CreateSaveGame();

	//Update the Subsystems keyMappings with a new Key set by the player
	UFUNCTION(BlueprintCallable)
		void UpdateMapping(FText DisplayName, FKey Key);

	//Return the Players Save Game
	UFUNCTION(BlueprintCallable)
		UControlsSaveGame* GetPlayerSaveGame() const {return ControlSaveGame;}

	//Update the SaveGames' KeyMappings
	UFUNCTION(BlueprintCallable)
		void UpdateKeyMappings();

	//Update the SaveGame's Mouse sensitivity level
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameMouseSensitivity();

	//Update the SaveGame's Mouse inverted X value
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameInvertMouseX();

	//Update the SaveGame's Mouse inverted Y value
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameInvertMouseY();

	//Update the SaveGame's Colour Deficiency type
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameColourDeficiency();

	//Update the SaveGame's Colour Deficiency severity
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameColourDeficiencySeverity();

	//Update the SaveGame's High score
	UFUNCTION(BlueprintCallable)
		void UpdateSaveGamePlayerHighScore(const bool bReset);

	//Set keyMappings to the SubSystems PlayerMappableKeyMappings
	//Delayed as it takes the Subsystem a Tick or more to update
	UFUNCTION()
		void DelayUpdatingKeyMappings();

	//Return the current KeyMappings
	UFUNCTION(BlueprintCallable)
		TArray<FEnhancedActionKeyMapping> GetKeyMappings() {return KeyMappings;}

	//Return whether the KeyMappings have already been loaded in the Settings User Widget
	UFUNCTION(BlueprintCallable)
		bool GetIsKeyLoaded() const {return bLoadedKeyMappings;}

	//Update whether the KeyMappings are loaded or not in the Settings User Widget
	UFUNCTION(BlueprintCallable)
		void SetIsKeyLoaded(const bool Value) {bLoadedKeyMappings = Value;} 
	
private:

	//Get the Players keyBindings
	//called after a short delay as this is the only way to get the Players' keyBindings in Unreal
	UFUNCTION()
		void SetKeyMappings();

	//Reference to the MainMenu User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuWidget;

	//Reference to the HUD User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HUDWidget;

	//Reference to the Settings User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SettingsWidget;

	//Reference to the Pause User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> PauseUserWidget;

	//Reference to the Credits User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CreditsUserWidgets;

	//Reference to the EndScreen User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> EndScreenWidget;

	//The created MainMenu User Widget
	UPROPERTY()
		UUserWidget* Menu;

	//The created HUD User Widget
	UPROPERTY()
		UUserWidget* HUD;

	//The created Settings User Widget
	UPROPERTY()
		UUserWidget* Settings;

	//The created Pause User Widget
	UPROPERTY()
		UUserWidget* PauseWidget;

	//The created Credits User Widget
	UPROPERTY()
		UUserWidget* CreditsWidget;

	//The created EndScreen User Widget
	UPROPERTY()
		UUserWidget* EndScreen;

	//Reference to the Players SaveGame
	UPROPERTY()
		UControlsSaveGame* ControlSaveGame;

	//Blueprint Reference to the Players SaveGame
	UPROPERTY(EditAnywhere)
		TSubclassOf<UControlsSaveGame> ControlSaveGameSubclass;

	//The name of the SaveGame slot
	UPROPERTY(EditAnywhere)
		FString ControlSaveGameName = "Controls";

	//Whether the Players KeyBindings have been loaded into the Settings User Widget
	UPROPERTY()
		bool bLoadedKeyMappings = false;
};
