// CCameron, JMills


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
	PrimaryActorTick.bCanEverTick = true;

	// Create cannons and attach to root
	CannonMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Mesh 1"));
	CannonMesh1->SetupAttachment(RootComponent);
	CannonMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon Mesh 2"));
	CannonMesh2->SetupAttachment(RootComponent);

	DroneSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Drone spawn sphere"));
	DroneSphere->SetupAttachment(RootComponent);
	DroneSphere->SetWorldLocation(FVector(200.0f, -100.0f, 50.0f));

	//Get a reference to the LevellingUp Component for the Player
	LevelComponent = CreateDefaultSubobject<ULevellingUpComponent>(TEXT("Level Component"));
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Audio Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Set looping timer to dissipate heat
	GetWorld()->GetTimerManager().SetTimer(HeatCooldownTimer, this, &APlayerCharacter::HeatTimerUp, HeatDissipationRate, true);
	
	// Save default max speed
	DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	//Update the Rockets Cooldown to be the Maximum Cooldown
	RocketCooldown = MaxRocketCooldown;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Destroy the player if health reaches 0
	if (PlayerHealth <= 0)
		Destroy();
	
	// Fire weapon if not on cooldown
	if (Firing && !OnCooldown)
 		FireWeapon();

	//If the Energy Cooldown UI is shown update the percentage of the Radial UI element 
	if (bShowEnergyCooldown)
		EnergyCooldownUI -= (DeltaTime / EnergyCooldown);

	//If the Rocket Cooldown UI is shown update the percentage of the Radial UI element
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
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(DroneSphere->GetComponentLocation());
	transform.SetRotation(GetActorRotation().Quaternion());
	// Spawn drone
	DroneRef = GetWorld()->SpawnActor<ADrone>(DroneClass, transform, SpawnParameters);
	DroneRef->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	DroneRef->bUseControllerRotationRoll = false;
}

//Update the Players score for this game
void APlayerCharacter::UpdatePlayerScore(const float Amount) const
{
	Cast<AMainPlayerController>(GetController())->SetPlayerScore(Amount);
}

//Stop showing the Hit indicator on the HUD
void APlayerCharacter::ResetPlayerHitIndicator()
{
	bShowHitIndicator = false;
}

// Fire the selected weapon and start cooldown
void APlayerCharacter::FireWeapon()
{
	int damage;
	int range;
	
	if (Weapon == WeaponType::Cannons)
	{
		PlayFireAudio();
		if (CannonOverheated) return;

		// If cannon is too hot start longer overheat timer and return
		if(CannonHeat >= MaxCannonHeat)
		{
			CannonOverheated = true;
			GetWorld()->GetTimerManager().SetTimer(OverheatCooldownTimer, this, &APlayerCharacter::CannonOverheatEnd, CannonOverheatCooldown, true);
			return;
		}

		// Add heat to cannon and start cooldown
		CannonHeat += CannonHeatIncrement;
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, CannonCooldown, false);

		// Set damage and range
		damage = CannonBaseDamage;
		range = CannonRange;
		bShowCannonCooldown = true;

		// Rotate cannons
		CannonMesh1->AddLocalRotation(FVector{ 0,0,0.1 }.Rotation());
		CannonMesh2->AddLocalRotation(FVector{ 0,0,-0.1 }.Rotation());
		
		// Raycast to crosshair position and apply damage
		Raycast(damage, range);

		OnCooldown = true;
	}
	else if (Weapon ==  WeaponType::Energy)
	{
		PlayFireAudio();

		// Start cooldown timer
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, EnergyCooldown, false);

		// Set damage and range
		bShowEnergyCooldown = true;
		damage = EnergyBaseDamage;
		range = EnergyRange;
		
		// Raycast to crosshair and apply damage
		Raycast(damage, range);

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
				transform.SetLocation(GetActorLocation() + FVector{ 0,0,200 });
				transform.SetRotation(GetActorRotation().Quaternion());

				// Spawn new rocket
				GetWorld()->SpawnActor<ARocket>(RocketClass, transform);

				//Fire a Rocket
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
					OnCooldown = true;
				}
				
				GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, RocketCooldown, false);
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

//Increase the amount of Rockets the Player can fire
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

// Swap between weapons in a cycle
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

	//Display the Hit indicator in the HUD User Widget
	bShowHitIndicator = true;

	//Remove the Hit indicator after a certain time
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetPlayerHitIndicator, 0.3f, false);

	//Reduce the amount of damage the Player will receive
	DamageAmount *= DamageReduction;

	//If the Players' dodge chance is less than a random float then make the Player take damage 
	if(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f) > DodgeChance)
		PlayerHealth -= DamageAmount;

	
	if (PlayerHealth <= 0.0f)
	{
		// Update Highscore 
		Cast<AMainPlayerController>(GetController())->UpdateSaveGamePlayerHighScore(false);

		//Load in the EndScreen User Widget
		Cast<AMainPlayerController>(GetController())->WidgetLoader(3);

		//Pause the Game and then Destroy the PlayerCharacter
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Destroy();
	}

	return PlayerHealth;
}

//If the Player has unlocked the 'Passive Health Regen' skill
//Call this function every couple of seconds to add Health to the Player
void APlayerCharacter::RegenerateHealth()
{
	if(PlayerHealth < PlayerMaxHealth)
		PlayerHealth += HealthRegenAmount;
}

//Increases the Players' damage reduction by an amount
void APlayerCharacter::IncreaseDamageReduction(float Amount)
{
	if((Amount > 0.0f && Amount <= 1.0f) || DamageReduction > 0.0f)
		DamageReduction -= Amount;
}

//Increase the Players' percentage chance to dodge an enemies shot
void APlayerCharacter::IncreaseDodgeChance(float Amount)
{
	if((Amount > 0.0f && Amount < 1.0f) || DodgeChance < 1.0f)
		DodgeChance += Amount;
}

//Increase the amount of Heat the Cannon can withstand
void APlayerCharacter::IncreaseMaxCannonHeat(float Amount)
{
	MaxCannonHeat *= Amount;
}

//Reset all Cooldown variables 
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

//Load in an additional Rocket
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

// Raycast and apply damage to enemy hit
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
	
	if(Weapon == WeaponType::Energy || DebugWeapons)
	{	
		FVector Weapon1Location = CannonMesh1->GetComponentLocation();
		FVector Weapon2Location = CannonMesh2->GetComponentLocation();
		DrawDebugLine(GetWorld(), Weapon1Location, End, FColor(0, 127, 255), false, 0.5, 0, 12.333);
		DrawDebugLine(GetWorld(), Weapon2Location, End, FColor(0, 127, 255), false, 0.5, 0, 12.333);
	}

	bool bRayHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams);

	// If an enemy is it apply damage
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