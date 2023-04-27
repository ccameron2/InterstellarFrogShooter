// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LevellingUpComponent.h"
#include "Rocket.h"
#include "Drone.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	Cannons = 0,
	Energy,
	Rocket
};

UCLASS()
class TEAMPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void MoveForwards(float AxisAmount);

	void Strafe(float AxisAmount);

	void LookUp(float AxisAmount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void Turn(float AxisAmount);

	void StartFireWeapon();
	void StopFireWeapon();
	void ChangeWeapon();
	
	//------------------------//
	//		Levelling Up	  //
	//------------------------//

	//Reference to the Players Levelling up Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ULevellingUpComponent* LevelComponent;

	//'Energy Weapon' skill
	UFUNCTION(BlueprintCallable)
		void UnlockEnergyWeapon() {bUnlockedEnergyWeapon = true;}

	//'Rocket Launcher' skill
	UFUNCTION(BlueprintCallable)
		void UnlockRocketLauncherWeapon() {bUnlockedRocketLauncher = true;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
		float PlayerHealth = 100.0f; //The Players current health

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
		int PlayerMaxHealth = 100; //Total amount of Health the Player can have

	UPROPERTY(EditAnywhere, Category="Health")
		float HealthRegenTimer = 2.5f; //How often to Regen Health when 'Passive Health Regen' is unlocked

	UPROPERTY(EditAnywhere, Category="Health")
		float HealthRegenAmount = 1.0f; //How much to add to the Players' Health when 'Passive Health Regen' is unlocked

	UPROPERTY()
		FTimerHandle HealthRegenTimerHandle;

	//Start a Timer to Regenerative health for the Player
	UFUNCTION(BlueprintCallable)
		void UnlockHealthRegen() {GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle, this, &APlayerCharacter::RegenerateHealth, HealthRegenTimer, true);}

	//If the Player has unlocked the 'Passive Health Regen' skill
	UFUNCTION()
		void RegenerateHealth();

	//Calculate the Players' Health as a percentage to display in the HUD User Widget
	UFUNCTION(BlueprintCallable)
		float GetPlayerHealthPercentage() const {return PlayerHealth / PlayerMaxHealth;}

	//Reduces the Damage the Player receives
	//Starts at 1 and then decreases when the Player unlocks additional Damage Reduction
	UPROPERTY(EditAnywhere, Category="Damage", BlueprintReadOnly)
		float DamageReduction = 1.0f;

	//Increases the Players' Damage reduction by an amount
	UFUNCTION(BlueprintCallable)
		void IncreaseDamageReduction(float Amount);

	//The Players percentage chance to dodge an enemies shot
	UPROPERTY(EditAnywhere, Category="Damage", BlueprintReadWrite)
		float DodgeChance = 0.0f;

	//Increase the Players' percentage chance to dodge an enemies shot
	UFUNCTION(BlueprintCallable)
		void IncreaseDodgeChance(float Amount);

	//Increase the amount of Heat the Cannon can withstand
	UFUNCTION(BlueprintCallable)
		void IncreaseMaxCannonHeat(float Amount);
	
	//------------------//
	//		Weapons		//
	//------------------//
	
	WeaponType Weapon =  WeaponType::Cannons;

	UFUNCTION(BlueprintCallable)
		WeaponType GetWeaponType() const {return Weapon;} 

	UFUNCTION(BlueprintCallable)
		void ReduceEnergyCooldown(const float ReductionAmount) {EnergyCooldown -= ReductionAmount;}

	//Increase the amount of Rockets the Player can fire
	UFUNCTION(BlueprintCallable)
		void IncreaseNumberOfRockets(const int Amount);
	
	UFUNCTION(BlueprintCallable)
		ADrone* GetDrone() const;

	UPROPERTY(EditAnywhere)
		int DamageMultiplier = 1;
	
	UPROPERTY(EditAnywhere, Category="Weapons", BlueprintReadOnly)
		float CannonBaseDamage = 5.0f; //Base Damage of Cannon without skills
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonRange = 8000.0f; //Cannon Range
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonCooldown = 0.01f; // Base Cooldown of Cannon without skills
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
		bool  CannonOverheated = false; // Base Overheat of Cannon without skills
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonOverheatCooldown = 1.0f; // Time to put Cannons on cooldown
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
		float CannonHeat = 0.0f; 
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonHeatIncrement = 5.0f; // Heat added per shot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
		float MaxCannonHeat = 100.0f; // Maximum amount of Heat the Cannon can withstand
	UPROPERTY(EditAnywhere, Category="Weapons")
		float HeatDissipationRate = 0.1f; // Time between decrementing CannonHeat

	UPROPERTY(EditAnywhere, Category="Weapons", BlueprintReadOnly)
		float EnergyBaseDamage = 50.0f; //Base Damage of Energy without skills
	UPROPERTY(EditAnywhere, Category="Weapons", BlueprintReadOnly)
		float EnergyCooldown = 2.0f; //Base Cooldown of Energy without skills
	UPROPERTY(EditAnywhere, Category="Weapons")
		float EnergyRange = 12000.0f; //Energy Range

	UPROPERTY(EditAnywhere, Category="Weapons", BlueprintReadOnly)
		float MaxRocketCooldown = 5.0f; //Base Cooldown between firing Rockets

	//Rocket Cooldown timer,
	//Needed for when the Player unlocks a new Rocket and fires it just before the Base Rocket timer runs out
	UPROPERTY()
		float RocketCooldown = 0;

	//Whether the Player is loading in a new Rocket
	UPROPERTY(BlueprintReadWrite)
		bool bRocketLoadingOnCooldown = false;

	//Whether the Players second Rocket has been loaded in after firing the First one,
	//Only needed when the 'Additional Rocket' skill is unlocked
	UPROPERTY()
		bool bSecondRocketReady = false;

	//The Maximum amount of Rockets the Player can have
	UPROPERTY(EditAnywhere, Category="Weapons", BlueprintReadOnly)
		int MaxRocketAmount = 1;

	//What Rocket the Player is currently on
	UPROPERTY(EditAnywhere, Category="Weapons")
		int CurrentRocketAmount = MaxRocketAmount;

	//Reference to the Rocket
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARocket> RocketClass;

	//Reference to the Drone
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADrone> DroneClass;
	//----------------------//
	//		Weapon UI		//
	//----------------------//

	//Whether to display the Energy Cooldown radial UI element
	UPROPERTY(BlueprintReadOnly)
		bool bShowEnergyCooldown = false;

	//Percentage of Energy Cooldown
	//Gets updated every tick Energy is on Cooldown
	UPROPERTY(BlueprintReadOnly)
		float EnergyCooldownUI = 1.0f;

	//Whether to display the Energy Cooldown radial UI element
	UPROPERTY(BlueprintReadOnly)
		bool bShowRocketLauncherCooldown = false;

	//Percentage of Rocket Cooldown
	//Gets updated every tick Rocket is on Cooldown
	UPROPERTY(BlueprintReadOnly)
		float RocketLauncherCooldownUI = 1.0f;

	//Return the Cannons Overheating Percentage
	//Used in the HUD User Widget for the Cannon Cooldown indicator
	UFUNCTION(BlueprintCallable)
		float GetCannonHeatUI() const {return CannonHeat / MaxCannonHeat;}

	//Spawn the Drone when the Player unlocks the 'Drone' skill
	UFUNCTION(BlueprintCallable)
		void SpawnDrone();

	//Whether to display the Energy Cooldown radial UI element
	UPROPERTY(BlueprintReadOnly)
		bool bShowCannonCooldown = true;

	//Whether to display that the PLayer has been hit by an enemy
	UPROPERTY(BlueprintReadOnly)
		bool bShowHitIndicator = false;

	//Whether the 'Energy Weapon' skill has been unlocked
	UPROPERTY(BlueprintReadOnly)
		bool bUnlockedEnergyWeapon = false;

	//Whether the 'Rocket Launcher' skill has been unlocked
	UPROPERTY(BlueprintReadOnly)
		bool bUnlockedRocketLauncher = false;

	//Whether the 'Additional Rocket' skill has been unlocked
	UPROPERTY(BlueprintReadOnly)
		bool bUnlockedAdditionalRockets = false;

	//How many enemies the Player has killed
	UPROPERTY(BlueprintReadOnly)
		int NumberOfKills = 0;

	//Add a new kill for the Player
	UFUNCTION()
		void IncrementKillCount() {++NumberOfKills;}

	//Update the Players score for this game
	UFUNCTION()
		void UpdatePlayerScore(const float Amount) const;

	//Reference to the Cannon Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CannonMesh1;

	//Reference to the Cannon Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CannonMesh2;

	float DefaultMaxSpeed;
	bool Underwater = false;
	bool SpeedReset = false;
	
private:
	
	//------------------//
	//		Audio		//
	//------------------//
	
	UPROPERTY(EditAnywhere, Category = Audio)
		UAudioComponent* FireAudioComponent;

	UPROPERTY(EditAnywhere, Category = Audio)
		TMap<WeaponType, USoundBase*> WeaponAudioMap;

	//Whether to display the LineTrace when firing a Weapon
	UPROPERTY(EditAnywhere)
		bool DebugWeapons;

	FTimerHandle WeaponCooldownTimer;
	bool OnCooldown = false;
	bool Firing = false;
	
	FTimerHandle HeatCooldownTimer;
	FTimerHandle OverheatCooldownTimer;
	
	UPROPERTY()
		ADrone* DroneRef;

	UFUNCTION()
		void ResetPlayerHitIndicator();

	void FireWeapon();
	void CooldownTimerUp();

	//Load an additional Rocket
	UFUNCTION()
		void LoadRocket();
	
	void CannonOverheatEnd();
	void HeatTimerUp();

	UFUNCTION()
		void Raycast(float damage, float range);
	
	void PlayFireAudio();	
};
