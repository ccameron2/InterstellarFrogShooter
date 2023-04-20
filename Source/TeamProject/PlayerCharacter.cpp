// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"
#include "Components/AudioComponent.h"
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
		RocketLauncherCooldownUI -= (DeltaTime / RocketCooldown);
}

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
	FTransform transform;
	transform.SetLocation(FVector{ 200,-50,100 });
	DroneRef = GetWorld()->SpawnActor<ADrone>(DroneClass, transform);
	DroneRef->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
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
			UE_LOG(LogTemp, Warning, TEXT("Overheated"));
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
	}
	else if (Weapon ==  WeaponType::Energy)
	{
		PlayFireAudio();
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, EnergyCooldown, false);

		bShowEnergyCooldown = true;
		Damage = EnergyBaseDamage;
		Range = EnergyRange;
		Raycast(Damage, Range);
	}
	else if (Weapon ==  WeaponType::Rocket)
	{
		PlayFireAudio();
		if(CurrentRocketAmount > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, RocketCooldown, false);

			FTransform transform;
			transform.SetLocation(GetActorLocation() + FVector{ 0,0,0 });
			transform.SetRotation(GetActorRotation().Quaternion());

			GetWorld()->SpawnActor<ARocket>(RocketClass, transform);
			CurrentRocketAmount -= 1;
			bShowRocketLauncherCooldown = true;
		}
	}

	OnCooldown = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Damage Reduction: %f"), DamageAmount);
	
	if(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f) > DodgeChance)
		PlayerHealth -= DamageAmount;

	if (PlayerHealth <= 0.0f)
	{
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
	CurrentRocketAmount = MaxRocketAmount; 
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
		UE_LOG(LogTemp, Warning, TEXT("Start: %s \n End: %s \n \n "), *Location.ToString(), *End.ToString());
		DrawDebugLine(GetWorld(), Location, End, FColor(0, 0, 255), true, -1, 0, 12.333);
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

