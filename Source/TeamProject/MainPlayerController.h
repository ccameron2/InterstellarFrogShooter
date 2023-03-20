// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
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

	void RebindCharacter(APlayerCharacter* playerCharacter);

	UFUNCTION(BlueprintCallable)
		void WidgetLoader(int index);

	void PauseGame();

	APlayerCharacter* Character;

	UPROPERTY()
		bool BuildMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInGame = false;

	UPROPERTY(BlueprintReadWrite)
		bool SkillTreeOpen = false;

private:


	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MainMenuWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SettingsWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> SkillTreeWidget;

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
		UUserWidget* SkillTree;

	UPROPERTY()
		UUserWidget* PauseWidget;

	UPROPERTY()
		UUserWidget* CreditsWidget;
};
