//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevellingUpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API ULevellingUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevellingUpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Current level of the player
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int CurrentXPLevel = 0;

	//The maximum level that the player can reach
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int LevelCap = 10;

	//How much to increase the XP amount by between levels
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "2.0", UIMin = "1.0", UIMax = "2.0"))
		float ExponentialValue = 1.5f;

	UPROPERTY(BlueprintReadOnly)
		bool bLevelledUp = false;

	//The current XP of the player
	UPROPERTY(BlueprintReadonly, EditAnywhere)
		float CurrentXP = 0.0f;

	//The current maximum amount of XP needed to level 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CurrentMaxXP = 83.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int AvailableSkillPoints = 0;

	//The percentage to be used by the Progress bars in the XP widgets
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float CurrentPercentage = 0.0f;

	//The amount of buffer XP for when the player levels up
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float BufferXP = 0.0f;

	//The starting level up requirement 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float BaseMaxXP = 83.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DamageMultiplier = 1.0f;

	//Function to calculate the current XP percentage for the progress bars in the XP widgets 
	UFUNCTION()
		void CalculatePercentage();

	//Function to calculate the Maximum XP that the player can get to before levelling up
	UFUNCTION()
		void CalculateMaxXP();

	//Function to give the player XP
	UFUNCTION(BlueprintCallable)
		void AddXP(float XPToAdd);

	//Function to return whether the Player has unlocked the MiniMap
	//Gets called in the HUD User Widget to set the Visibility of the MiniMap
	UFUNCTION(BlueprintCallable)
		bool MiniMapUnlocked() const {return bMiniMap;}

	//Function to unlock the MiniMap for the Player
	//Gets called in the UnlockDrone Skill
	UFUNCTION(BlueprintCallable)
		void UnlockMiniMap() {bMiniMap = true;}

	//Function to level up the player
	UFUNCTION()
		void LevelUP();

	//Function to Reset the 'Levelled Up' text being displayed in the HUD User Widget
	UFUNCTION()
		void ResetLevelledUpBool();

private:
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

	//Whether the Player has access to the MiniMaps
	UPROPERTY()
		bool bMiniMap = false;
		
};
