// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"
#include "MainPlayerController.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CannonMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Mesh 1"));
	CannonMesh1->SetupAttachment(RootComponent);
	CannonMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Mesh 2"));
	CannonMesh2->SetupAttachment(RootComponent);

	LevelComponent = CreateDefaultSubobject<ULevellingUpComponent>(TEXT("Level Component"));
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Audio Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(HeatCooldownTimer, this, &APlayerCharacter::HeatTimerUp, HeatDissipationRate, true);
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	RocketCooldown = MaxRocketCooldown;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerHealth <= 0)
		Destroy();
	
	if (Firing && !OnCooldown)
 		FireWeapon();
	
	if (bShowEnergyCooldown)
		EnergyCooldownUI -= (DeltaTime / EnergyCooldown);
	
	if(bShowRocketLauncherCooldown)
		RocketLauncherCooldownUI -= (DeltaTime / MaxRocketCooldown);

	// Slow the character if underwater
	if (Underwater)
	{
		 GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed / 2;
		 SpeedReset = false;
	}
	else
	{
		if (!SpeedReset)
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
			SpeedReset = true;
		}
	}
}

//Allows the player to move the character forwards and backwards
void APlayerCharacter::MoveForwards(float AxisAmount)
{
	AddMovementInput(GetActorForwardVector() * AxisAmount);
}

//Allows the player to move the character left and right
void APlayerCharacter::Strafe(float AxisAmount)
{
	AddMovementInput(GetActorRightVector() * AxisAmount);
}

//Allows the player to rotate the character up and down
void APlayerCharacter::LookUp(float AxisAmount)
{
	AddControllerPitchInput(AxisAmount);
}

//Allows the player to rotate the character left and right
void APlayerCharacter::Turn(float AxisAmount)
{
	AddControllerYawInput(AxisAmount);
}

void APlayerCharacter::SpawnDrone()
{
	//Make sure the Drone is able to be spawned no matter what
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform Transform;
	
	Transform.SetLocation(GetActorLocation() + FVector{ 200,-100, 50 });
	
	DroneRef = GetWorld()->SpawnActor<ADrone>(DroneClass, Transform, SpawnParameters);
	
	DroneRef->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	DroneRef->bUseControllerRotationRoll = false;
}

//Update the Players total score for the current game
//by calling the SetPlayerScore function in the PlayerController 
void APlayerCharacter::UpdatePlayerScore(const float Amount)
{
	Cast<AMainPlayerController>(GetController())->SetPlayerScore(Amount);
}

void APlayerCharacter::ResetPlayerHitIndicator()
{
	bShowHitIndicator = false;
}

void APlayerCharacter::FireWeapon()
{
	int Damage;
	int Range;
	if (Weapon == WeaponType::Cannons)
	{
		PlayFireAudio();
		if (CannonOverheated) return;
		if(CannonHeat >= MaxCannonHeat)
		{
			CannonOverheated = true;
			GetWorld()->GetTimerManager().SetTimer(OverheatCooldownTimer, this, &APlayerCharacter::CannonOverheatEnd, CannonOverheatCooldown, true);
			return;
		}
		CannonHeat += CannonHeatIncrement;
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, CannonCooldown, false);
		Damage = CannonBaseDamage;
		Range = CannonRange;
		bShowCannonCooldown = true;
		CannonMesh1->AddLocalRotation(FVector{ 0,0,0.1 }.Rotation());
		CannonMesh2->AddLocalRotation(FVector{ 0,0,-0.1 }.Rotation());
		Raycast(Damage, Range);

		OnCooldown = true;
	}
	else if (Weapon ==  WeaponType::Energy)
	{
		PlayFireAudio();
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, EnergyCooldown, false);

		bShowEnergyCooldown = true;
		Damage = EnergyBaseDamage;
		Range = EnergyRange;
		
		Raycast(Damage, Range);
		OnCooldown = true;
	}
	else if (Weapon ==  WeaponType::Rocket)
	{
		//Only allow the firing of Rockets if the Player is not loading in a new Rocket
		if(!bRocketLoadingOnCooldown)
		{
			PlayFireAudio();

			//Makes sure that there is a rocket to fire
			if(CurrentRocketAmount > 0)
			{
				
				FTransform transform;
				transform.SetLocation(GetActorLocation() + FVector{ 0,0,0 });
				transform.SetRotation(GetActorRotation().Quaternion());

				
				GetWorld()->SpawnActor<ARocket>(RocketClass, transform);


				CurrentRocketAmount -= 1;

				//Allow the HUD Widget to display the Rocket Cooldown
				bShowRocketLauncherCooldown = true;

				//If the player has the additional Rocket skill
				if(CurrentRocketAmount > 0)
				{
					//Make sure this function cannot be called while a new Rocket is being loaded into the "Mech"
					bRocketLoadingOnCooldown = true;
					
					FTimerHandle RocketLoadingTimerHandle;

					//Set a timer to Load in the new Rocket
					GetWorld()->GetTimerManager().SetTimer(RocketLoadingTimerHandle, this, &APlayerCharacter::LoadRocket, RocketCooldown / 2, false);

					//Allow the Mech to fire again if there is still another Rocket available
					OnCooldown = false;
				}
				else
				{
					//Update the Rocket Cooldown timer based on when the Player decides to fire the newly loaded in Rocket
					if(bSecondRocketReady)
					{
						//Multiplying by the current Cooldown value as it is between 0 and 1 and therefore provides
						//a great way to get the percentage of the Cooldown left
						RocketCooldown *= RocketLauncherCooldownUI;
					}

					//Set a timer to Cooldown the Rocket Launcher
					GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, RocketCooldown, false);
					OnCooldown = true;
				}
			}
		}
	}
}

void APlayerCharacter::StartFireWeapon()
{
	Firing = true;
}

void APlayerCharacter::StopFireWeapon()
{
	Firing = false;
}

void APlayerCharacter::UpdateDeveloperMode(bool Value)
{
	bDeveloperMode = Value;
}

void APlayerCharacter::IncreaseNumberOfRockets(const int Amount)
{
	MaxRocketAmount += Amount;
	CurrentRocketAmount = MaxRocketAmount;
	bUnlockedAdditionalRockets = true;
}

//Get a reference to the Players Drone to enable the Drone Skills:
// - Drone_Gun_Skill
// - Drone_Increase_Damage_Skill
// - Drone_Increase_Firerate_Skill
// - Drone_Increase_Range_Skill
ADrone*	APlayerCharacter::GetDrone() const
{
	if(DroneRef)
	{
		return DroneRef;
	}
	return nullptr;
}

void APlayerCharacter::ChangeWeapon()
{
	if (Weapon ==  WeaponType::Cannons && bUnlockedEnergyWeapon)
	{
		Weapon =  WeaponType::Energy;
		bShowCannonCooldown = false;
	}
	else if (Weapon ==  WeaponType::Energy)
	{
		if (bUnlockedRocketLauncher)
		{
			Weapon =  WeaponType::Rocket;
		}
		else
		{
			Weapon =  WeaponType::Cannons;
		}
		bShowCannonCooldown = true;
	}
	else if (Weapon ==  WeaponType::Rocket) Weapon =  WeaponType::Cannons;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bShowHitIndicator = true;
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetPlayerHitIndicator, 0.3f, false);

	DamageAmount *= DamageReduction;
	
	if(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f) > DodgeChance)
		PlayerHealth -= DamageAmount;

	if (PlayerHealth <= 0.0f)
	{
		Cast<AMainPlayerController>(GetController())->UpdateSaveGamePlayerHighScore(false);
		Cast<AMainPlayerController>(GetController())->WidgetLoader(3);

		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Destroy();
	}

	return PlayerHealth;
}

void APlayerCharacter::RegenerateHealth()
{
	if(PlayerHealth < PlayerMaxHealth)
		PlayerHealth += HealthRegenAmount;
}

void APlayerCharacter::IncreaseDamageReduction(float Amount)
{
	if((Amount > 0.0f && Amount <= 1.0f) || DamageReduction > 0.0f)
		DamageReduction -= Amount;
}

void APlayerCharacter::IncreaseDodgeChance(float Amount)
{
	if((Amount > 0.0f && Amount < 1.0f) || DodgeChance < 1.0f)
		DodgeChance += Amount;
}

void APlayerCharacter::IncreaseMaxCannonHeat(float Amount)
{
	MaxCannonHeat *= Amount;
}

void APlayerCharacter::CooldownTimerUp()
{
	bShowEnergyCooldown = false;
	bShowRocketLauncherCooldown = false;
	OnCooldown = false;
	EnergyCooldownUI = 1.0f;
	RocketLauncherCooldownUI = 1.0f;
	bSecondRocketReady = false;
	CurrentRocketAmount = MaxRocketAmount;
	RocketCooldown = MaxRocketCooldown;
}

void APlayerCharacter::LoadRocket()
{
	bRocketLoadingOnCooldown = false;
	bSecondRocketReady = true;
}

void APlayerCharacter::CannonOverheatEnd()
{
	if (CannonHeat < MaxCannonHeat / 2)
	{
		CannonOverheated = false;
		GetWorldTimerManager().ClearTimer(OverheatCooldownTimer);
	}
}

void APlayerCharacter::HeatTimerUp()
{
	if(CannonHeat > 0) CannonHeat--;
}

void APlayerCharacter::Raycast(float damage, float range)
{

	AController* ControllerReference = GetController();

	FVector Location;
	FRotator Rotation;
	FVector End;
	FHitResult Hit;

	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this);
	ControllerReference->GetPlayerViewPoint(Location, Rotation);
	End = Location + (Rotation.Vector() * range);

	if (DebugWeapons)
	{
		FVector Weapon1Location = CannonMesh1->GetComponentLocation();
		FVector Weapon2Location = CannonMesh2->GetComponentLocation();
		DrawDebugLine(GetWorld(), Weapon1Location, End, FColor(0, 127, 255), false, 2.5, 0, 12.333);
		DrawDebugLine(GetWorld(), Weapon2Location, End, FColor(0, 127, 255), false, 2.5, 0, 12.333);
	}
	if(Weapon == WeaponType::Energy)
	{
		FVector Weapon1Location = CannonMesh1->GetComponentLocation();
		FVector Weapon2Location = CannonMesh2->GetComponentLocation();
		DrawDebugLine(GetWorld(), Weapon1Location, End, FColor(0, 127, 255), false, 1, 0, 12.333);
		DrawDebugLine(GetWorld(), Weapon2Location, End, FColor(0, 127, 255), false, 1, 0, 12.333);
	}

	bool bRayHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams);

	if (bRayHit && Hit.GetActor() != nullptr)
	{
		if (Hit.GetActor()->GetClass()->IsChildOf(AAICharacter::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(
				Hit.GetActor(),
				damage * LevelComponent->DamageMultiplier * DamageMultiplier, // Damage Amount 
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

void APlayerCharacter::PlayFireAudio()
{
	if(WeaponAudioMap.Contains(Weapon))
	{
		FireAudioComponent->SetSound(*WeaponAudioMap.Find(Weapon));
		FireAudioComponent->Play();
	}
}