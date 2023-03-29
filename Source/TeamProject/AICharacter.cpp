// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "AIHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationInvokerComponent.h"
#include "PlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigation Invoker"));
	
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	State = EAIState::Decision;
	Reasons = EDecisionReasons::None;
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
	if(TargetActor)
	{
		FVector Location;
		FRotator Rotation;
		GetActorEyesViewPoint(Location, Rotation);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, TargetActor->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, Params);

		if(bSuccess)
		{
			AActor* HitActor = Hit.GetActor();
			if(HitActor != nullptr)
			{
				if(HitActor->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("Player Shot"));
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

	if(State != EAIState::Run) // Allows the AI to keep running when being shot
	{
		State = EAIState::Decision;
		Reasons = EDecisionReasons::BeingShot;
	}
	
	ShootFromLocation = DamageCauser->GetActorLocation(); // Would Rather detect the direction that the shoot came from rather than get the players position
	
	UE_LOG(LogTemp, Warning, TEXT("TakingDamage"));
	
	FVector SpawnLocation = GetActorLocation();
	FString DamageString = FString::SanitizeFloat(DamageAmount);

	SpawnLocation.X += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Y += UKismetMathLibrary::RandomFloatInRange(-25.0f, 25.0f);
	SpawnLocation.Z += 150.0f;

	FRotator SpawnRotation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorRotation();

	AHitpointText* Hitpoint =  GetWorld()->SpawnActor<AHitpointText>(HitPointText, SpawnLocation, SpawnRotation);
	Hitpoint->HitpointsText->SetText(FText::FromString(DamageString));
	Hitpoint->HitpointsText->SetTextRenderColor(FColor::Red);

	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		XPAmount = UKismetMathLibrary::RandomFloatInRange(MinXPAmount, MaxXPAmount);
		Cast<APlayerCharacter>(DamageCauser)->LevelComponent->AddXP(XPAmount);
		Cast<APlayerCharacter>(DamageCauser)->IncrementKillCount();
		
		Destroy();
	}

	return Health;
}

