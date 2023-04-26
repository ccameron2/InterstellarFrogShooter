// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "AIHelpers.h"
#include "MainPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationInvokerComponent.h"
#include "PlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigation Invoker"));

	FrogMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frog Mesh"));
	FrogMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set up frog colours
	BodyInstanceMaterial = FrogMesh->CreateDynamicMaterialInstance(0);
	ToesAndBellyInstanceMaterial = FrogMesh->CreateDynamicMaterialInstance(2);
	const uint32_t ColourIndex = FMath::RandRange(0, BodyColourArray.Num() - 1);
	if(BodyColourArray.IsValidIndex(ColourIndex) && ToesAndBellyColourArray.IsValidIndex(ColourIndex))
	{
		BodyInstanceMaterial->SetVectorParameterValue(FName("Colour"), BodyColourArray[ColourIndex]);
		ToesAndBellyInstanceMaterial->SetVectorParameterValue(FName("Colour"), ToesAndBellyColourArray[ColourIndex]);
		if(ColourIndex == 0)
			Damage *= DamageTypeMultiplier;
		else if (ColourIndex == 1)
			MaxHealth *= HealthTypeMultiplier;
		else if (ColourIndex == 2)
			GetCharacterMovement()->MaxWalkSpeed  *= SpeedTypeMultiplier;
	}

	// Set health to the max health
	Health = MaxHealth;

	// Set default states
	State = EAIState::Decision;
	Reasons = EDecisionReasons::None;

	// Calculate which drop to drop on death
	CalculateDrop();
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::Shoot(AActor* TargetActor)
{
	// If can see the target actor (player)
	if(TargetActor)
	{
		FVector Location;
		FRotator Rotation;
		GetActorEyesViewPoint(Location, Rotation); // Sets up the location and rotation to the frogs eyes, this is where the ray will shoot from

		FHitResult Hit;
		// Params to stop ray from hitting self
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		// Shoots the ray
		bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, TargetActor->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, Params);

		// If ray has hit something check if it is the player actor than apply the set damage
		if(bSuccess)
		{
			AActor* HitActor = Hit.GetActor();
			if(HitActor != nullptr)
			{
				if(HitActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
				{
					UGameplayStatics::ApplyDamage(HitActor, Damage,
						GetInstigatorController(), this, UDamageType::StaticClass());
				}
			}
		}
	}
}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// Function called when shoot by another actor (player)
	
	if(State != EAIState::Run) // Allows the AI to keep running when being shot
	{
		State = EAIState::Decision;
		Reasons = EDecisionReasons::BeingShot;
	}

	// Gets the location of where the Ai was shot from 
	ShootFromLocation = DamageCauser->GetActorLocation(); // Would Rather detect the direction that the shoot came from rather than get the players position

	// Sets up the location of the damage UI
	FVector SpawnLocation = GetActorLocation();
	FString DamageString = FString::SanitizeFloat(DamageAmount);
	SpawnLocation.X += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Y += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Z += 150.0f;

	// Spawn the hit point text with the damage
	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		FRotator SpawnRotation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorRotation();
		AHitpointText* Hitpoint = GetWorld()->SpawnActor<AHitpointText>(HitPointText, SpawnLocation, SpawnRotation);
		Hitpoint->HitpointsText->SetText(FText::FromString(DamageString));
		Hitpoint->HitpointsText->SetTextRenderColor(FColor::Red);
	}

	// Remove the damage delt from the health
	Health -= DamageAmount;

	// Check if the AI is dead
	if (Health <= 0.0f)	
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		XPAmount = UKismetMathLibrary::RandomFloatInRange(MinXPAmount, MaxXPAmount);
		if(Player)
		{
			// Update the players stats
			Player->LevelComponent->AddXP(XPAmount);
			Player->IncrementKillCount();
			
			Player->UpdatePlayerScore(XPAmount * ScoreMultiplier);
		}

		// Spawn the selected drop
		SpawnDrop();

		// Destroy the actor
		Destroy();
	}
	return Health;
}

void AAICharacter::SpawnDrop()
{
	if(!PickupMap.IsEmpty())
	{
		// Spawn Pick Up
		if(SelectedType != EPickUpType::None)
		{
			TSubclassOf<ABasePickUpActor> PickupToSpawn = *PickupMap.Find(SelectedType)->PickupClass;
			if(PickupToSpawn != nullptr)
			{
				GetWorld()->SpawnActor<ABasePickUpActor>(PickupToSpawn, GetActorLocation(), GetActorRotation());
			}
		}
	}
}

void AAICharacter::CalculateDrop()
{
	if(!PickupMap.IsEmpty())
	{
		// Calculate Total Probability
		// This allows the drops to be scalable and not be hard coded to 100%
		for(auto elem : PickupMap)
		{
			PickupTotalProbability += elem.Value.Rate;
		}

		// SelectDrop
		// Selects a random number in the range of the probability
		// And find the correct type by checking if the number is less than the chance
		// then spawned the selected type based on the element
		int cumulativeChance = 0;
		const int Number = FMath::RandRange(0, PickupTotalProbability);
		for(auto elem : PickupMap)
		{
			cumulativeChance += elem.Value.Rate;
			if(Number < cumulativeChance)
			{
				SelectedType = elem.Key;
				break; // Breaks out of for loop one found
			}
		}
	}
}

