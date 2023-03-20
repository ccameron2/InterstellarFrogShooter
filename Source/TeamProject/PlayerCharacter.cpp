// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "TestActor.h"
#include "Kismet/GameplayStatics.h"
#include "AICharacter.h"

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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(HeatCooldownTimer, this, &APlayerCharacter::HeatTimerUp, HeatDissipationRate, true);
	//SpawnDrone();
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

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::StartFireWeapon);
	InputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::StopFireWeapon);
	InputComponent->BindAction(TEXT("ChangeWeapon"), IE_Pressed, this, &APlayerCharacter::ChangeWeapon);

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

void APlayerCharacter::SpawnDrone()
{
	FTransform transform;
	transform.SetLocation(GetActorLocation() + FVector{ -100,50,500 });
	DroneRef = GetWorld()->SpawnActor<ADrone>(DroneClass, transform);
	DroneRef->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
}

void APlayerCharacter::ResetPlayerHitIndicator()
{
	bShowHitIndicator = false;
}

void APlayerCharacter::FireWeapon()
{
	int damage = 0;
	int range = 0;
	if (Weapon == Cannons)
	{		
		if(CannonHeat > MaxCannonHeat) return;
		CannonHeat += CannonHeatIncrement;
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, CannonCooldown, false);
		damage = CannonBaseDamage;
		range = CannonRange;
		bShowCannonCooldown = true;
		CannonMesh1->AddLocalRotation(FVector{ 0,0,0.1 }.Rotation());
		CannonMesh2->AddLocalRotation(FVector{ 0,0,-0.1 }.Rotation());
		Raycast(damage, range);
	}
	else if (Weapon == Energy)
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponCooldownTimer, this, &APlayerCharacter::CooldownTimerUp, EnergyCooldown, false);
		bShowEnergyCooldown = true;
		damage = EnergyBaseDamage;
		range = EnergyRange;
		Raycast(damage, range);
	}
	else if (Weapon == Rocket)
	{
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

void APlayerCharacter::ChangeWeapon()
{
	if (Weapon == Cannons && bUnlockedEnergyWeapon)
	{
		Weapon = Energy;
		bShowCannonCooldown = false;
	}
	else if (Weapon == Energy)
	{
		if (bUnlockedRocketLauncher)
		{
			Weapon = Rocket;
		}
		else
		{
			Weapon = Cannons;
		}
		bShowCannonCooldown = true;
	}
	else if (Weapon == Rocket) Weapon = Cannons;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bShowHitIndicator = true;
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::ResetPlayerHitIndicator, 0.3f, false);

	PlayerHealth -= DamageAmount;

	if (PlayerHealth <= 0.0f)
	{
		Destroy();
	}

	return PlayerHealth;
}

//Allows the player to rotate the character left and right
void APlayerCharacter::Turn(float AxisAmount)
{
	AddControllerYawInput(AxisAmount);
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
				damage * LevelComponent->DamageMultiplier, // Damage Amount 
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

