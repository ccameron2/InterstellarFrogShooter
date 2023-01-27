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

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Player Mesh"));
	PlayerMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::FireWeapon);
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

void APlayerCharacter::FireWeapon()
{
	AController* ControllerReference = GetController();

	FVector Location;
	FRotator Rotation;
	FVector End;
	FHitResult Hit;

	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this);
	ControllerReference->GetPlayerViewPoint(Location, Rotation);
	End = Location + (Rotation.Vector() * 10000);

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
				10.0f, // Damage Amount 
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

//Allows the player to rotate the character left and right
void APlayerCharacter::Turn(float AxisAmount)
{
	AddControllerYawInput(AxisAmount);
}

