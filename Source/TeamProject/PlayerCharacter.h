// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LevellingUpComponent.h"
#include "Rocket.h"
#include "Drone.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM() enum WeaponType
{
	Cannons,
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForwards(float AxisAmount);

	void Strafe(float AxisAmount);

	void LookUp(float AxisAmount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void Turn(float AxisAmount);


	//------------------------//
	//		Levelling Up	  //
	//------------------------//
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ULevellingUpComponent* LevelComponent;

	UFUNCTION(BlueprintCallable)
		void UnlockEnergyWeapon() {bUnlockedEnergyWeapon = true;}

	UFUNCTION(BlueprintCallable)
		void UnlockRocketLauncherWeapon() {bUnlockedRocketLauncher = true;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
		float PlayerHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
		int PlayerMaxHealth = 100;

	UPROPERTY(EditAnywhere, Category="Health")
		float HealthRegenTimer = 2.5f;

	UPROPERTY(EditAnywhere, Category="Health")
		float HealthRegenAmount = 1.0f;

	UPROPERTY()
		FTimerHandle HealthRegenTimerHandle;

	UFUNCTION(BlueprintCallable)
		void UnlockHealthRegen() {GetWorld()->GetTimerManager().SetTimer(HealthRegenTimerHandle, this, &APlayerCharacter::RegenerateHealth, HealthRegenTimer, true);}

	UFUNCTION()
		void RegenerateHealth();
	
	UFUNCTION(BlueprintCallable)
		float GetPlayerHealthPercentage() const {return PlayerHealth / PlayerMaxHealth;}
	
	UPROPERTY(EditAnywhere, Category="Damage")
		float DamageReduction = 1.0f;

	UFUNCTION(BlueprintCallable)
		void IncreaseDamageReduction(float Amount);

	UPROPERTY(EditAnywhere, Category="Damage")
		float DodgeChance = 0.0f;

	UFUNCTION(BlueprintCallable)
		void IncreaseDodgeChance(float Amount);

	//--------------------------//
	//		Developer Mode		//
	//--------------------------//

	UFUNCTION()
		void UpdateDeveloperMode(bool Value);

	UPROPERTY(BlueprintReadOnly)
		bool bDeveloperMode = false;

	
	//------------------//
	//		Weapons		//
	//------------------//
	WeaponType Weapon = Cannons;

	UFUNCTION(BlueprintCallable)
		WeaponType GetWeaponType() const {return Weapon;} 

	UFUNCTION(BlueprintCallable)
		void ReduceEnergyCooldown(const float ReductionAmount) {EnergyCooldown -= ReductionAmount;}

	UFUNCTION(BlueprintCallable)
		void IncreaseNumberOfRockets(const int Amount)
	{
		MaxRocketAmount += Amount;
		CurrentRocketAmount = MaxRocketAmount;
		UE_LOG(LogTemp, Warning, TEXT("Increased Rockets"));
	}
	
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonBaseDamage = 5.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonRange = 8000.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonCooldown = 0.01f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonHeat = 0.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float CannonHeatIncrement = 1.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float MaxCannonHeat = 100.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float HeatDissipationRate = 0.1f;

	UPROPERTY(EditAnywhere, Category="Weapons")
		float EnergyBaseDamage = 50.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float EnergyCooldown = 2.0f;
	UPROPERTY(EditAnywhere, Category="Weapons")
		float EnergyRange = 16000.0f;

	UPROPERTY(EditAnywhere, Category="Weapons")
		float RocketCooldown = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Weapons")
		int MaxRocketAmount = 1;

	UPROPERTY(EditAnywhere, Category="Weapons")
		int CurrentRocketAmount = MaxRocketAmount;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARocket> RocketClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ADrone> DroneClass;
	//----------------------//
	//		Weapon UI		//
	//----------------------//

	UPROPERTY(BlueprintReadOnly)
		bool bShowEnergyCooldown = false;

	UPROPERTY(BlueprintReadOnly)
		float EnergyCooldownUI = 1.0f;

	UPROPERTY(BlueprintReadOnly)
		bool bShowRocketLauncherCooldown = false;

	UPROPERTY(BlueprintReadOnly)
		float RocketLauncherCooldownUI = 1.0f;

	UFUNCTION(BlueprintCallable)
		float GetCannonHeatUI() const {return CannonHeat / MaxCannonHeat;}
	
	UFUNCTION(BlueprintCallable)
		void SpawnDrone();

	UPROPERTY(BlueprintReadOnly)
		bool bShowCannonCooldown = true;

	UPROPERTY(BlueprintReadOnly)
		bool bShowHitIndicator = false;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CannonMesh1;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CannonMesh2;

	UPROPERTY(EditAnywhere)
		bool DebugWeapons;

	FTimerHandle WeaponCooldownTimer;
	bool OnCooldown = false;
	bool Firing = false;

	FTimerHandle HeatCooldownTimer;
	
	UPROPERTY()
		ADrone* DroneRef;

	UFUNCTION()
		void ResetPlayerHitIndicator();

	UPROPERTY()
		bool bUnlockedEnergyWeapon = false;

	UPROPERTY()
		bool bUnlockedRocketLauncher = false;

	void FireWeapon();
	void StartFireWeapon();
	void StopFireWeapon();
	void ChangeWeapon();
	void CooldownTimerUp();
	void HeatTimerUp();
	void Raycast(float damage, float range);
};
