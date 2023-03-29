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

	UFUNCTION(BlueprintCallable)
		void WidgetLoader(int index);

	void PauseGame();

	void AddOrRemoveSkillTree();

	APlayerCharacter* Character;

	//--------------------------//
	//		Input System		//
	//--------------------------//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
		UInputMappingContext* DefaultMappingContext;

	UPROPERTY(BlueprintReadOnly)
		TArray<FEnhancedActionKeyMapping> KeyMappings;

	UPROPERTY()
		UEnhancedInputLocalPlayerSubsystem* Subsystem;

	//----------------------//
	//	Input Actions		//
	//----------------------//

	//Input Action for Pausing the Game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
		UInputAction* PauseAction;

	//Input Action for Pausing the Game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* TurnAction;
	
	//Input Action for Pausing the Game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	UInputAction* LookUpAction;
	
	//Input Action for Turning Left
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* ChangeWeaponAction;

	//Input Action for Turning Right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* FireAction;

	//Input Action for Jumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	//Input Action for Moving Left and Right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* StrafeAction;

	//Input Action for Moving Left and Right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* ForwardAction;

	//Input Action for Moving Left and Right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* SkillTreeAction;


	
	UPROPERTY()
		bool BuildMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInGame = false;

	UPROPERTY(BlueprintReadWrite)
		bool SkillTreeOpen = false;

	UFUNCTION()
		void UpdateDeveloperMode();

	UPROPERTY(BlueprintReadOnly)
		bool bDeveloperMode = false;

	//--------------------------//
	//		Mouse Settings		//
	//--------------------------//

	UPROPERTY(BlueprintReadWrite)
		float InvertMouseXValue = 1.0f;

	UPROPERTY(BlueprintReadWrite)
		float InvertMouseYValue = 1.0f;

	UPROPERTY(BlueprintReadWrite)
		float MouseSensitivity = 1.0f;

	UFUNCTION(BlueprintCallable)
		void InvertMouseX() {InvertMouseXValue *= -1;} 

	UFUNCTION(BlueprintCallable)
		void InvertMouseY() {InvertMouseYValue *= -1;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> SkillTreeWidget;

	UPROPERTY(BlueprintReadWrite)
		UUserWidget* SkillTree;

	UFUNCTION(BlueprintCallable)
		void SaveGame();

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

	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameMouseSensitivity();

	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameInvertMouseX();

	UFUNCTION(BlueprintCallable)
		void UpdateSaveGameInvertMouseY();

	//Set keyMappings to the SubSystems PlayerMappableKeyMappings
	//Delayed as it takes the Subsystem a Tick or more to update
	UFUNCTION()
		void DelayUpdatingKeyMappings();

	//Return the current KeyMappings
	UFUNCTION(BlueprintCallable)
		TArray<FEnhancedActionKeyMapping> GetKeyMappings() {return KeyMappings;}

	UFUNCTION(BlueprintCallable)
		bool GetIsKeyLoaded() const {return bLoadedKeyMappings;}

	UFUNCTION(BlueprintCallable)
		void SetIsKeyLoaded(const bool Value) {bLoadedKeyMappings = Value;} 
	
private:

	UFUNCTION()
		void SetKeyMappings();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SettingsWidget;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> PauseUserWidget;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CreditsUserWidgets;

	UPROPERTY()
		UUserWidget* Menu;

	UPROPERTY()
		UUserWidget* HUD;

	UPROPERTY()
		UUserWidget* Settings;

	UPROPERTY()
		UUserWidget* PauseWidget;

	UPROPERTY()
		UUserWidget* CreditsWidget;

	//Reference to the Players SaveGame
	UPROPERTY()
		UControlsSaveGame* ControlSaveGame;

	//Blueprint Reference to the Players SaveGame
	UPROPERTY(EditAnywhere)
		TSubclassOf<UControlsSaveGame> ControlSaveGameSubclass;

	UPROPERTY(EditAnywhere)
		FString ControlSaveGameName = "Controls";

	UPROPERTY()
		bool bLoadedKeyMappings = false;
};
