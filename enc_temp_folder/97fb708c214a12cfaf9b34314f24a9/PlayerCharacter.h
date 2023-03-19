// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LevellingUpComponent.h"
#include "Rocket.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PlayerMaxHealth = 100;


	//------------------//
	//		Weapons		//
	//------------------//
	WeaponType Weapon = Cannons;

	UFUNCTION(BlueprintCallable)
		WeaponType GetWeaponType() const {return Weapon;} 

	UPROPERTY(EditAnywhere)
		float CannonBaseDamage = 2.0f;
	UPROPERTY(EditAnywhere)
		float CannonRange = 5000.0f;
	UPROPERTY(EditAnywhere)
		float CannonCooldown = 0.01f;
	UPROPERTY(EditAnywhere)
		float CannonHeat = 0.0f;
	UPROPERTY(EditAnywhere)
		float CannonHeatIncrement = 1.0f;
	UPROPERTY(EditAnywhere)
		float MaxCannonHeat = 100.0f;
	UPROPERTY(EditAnywhere)
		float HeatDissipationRate = 0.1f;

	UPROPERTY(EditAnywhere)
		float EnergyBaseDamage = 10.0f;
	UPROPERTY(EditAnywhere)
		float EnergyCooldown = 2.0f;
	UPROPERTY(EditAnywhere)
		float EnergyRange = 10000.0f;

	UPROPERTY(EditAnywhere)
		float RocketCooldown = 10.0f;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARocket> RocketClass;

	//----------------------//
	//		Weapon UI		//
	//----------------------//

	UPROPERTY(BlueprintReadOnly)
		bool bShowEnergyCooldown = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float EnergyCooldownUI = 1.0f;

	UFUNCTION(BlueprintCallable)
		float GetCannonHeatUI() const {return CannonHeat / MaxCannonHeat;}

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


	UFUNCTION()
		void ResetPlayerHitIndicator();

	UPROPERTY()
		bool bUnlockedEnergyWeapon = false;

	void FireWeapon();
	void StartFireWeapon();
	void StopFireWeapon();
	void ChangeWeapon();
	void CooldownTimerUp();
	void HeatTimerUp();
	void Raycast(float damage, float range);
};
