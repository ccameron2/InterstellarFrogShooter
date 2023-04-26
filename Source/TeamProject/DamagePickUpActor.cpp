// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePickUpActor.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"

ADamagePickUpActor::ADamagePickUpActor()
{
	PickUpType = EPickUpType::Damage;
}

void ADamagePickUpActor::OnPickUp(APlayerCharacter* Character)
{
	Super::OnPickUp(Character);

	
	PlayerCharacter = Character;
	PreviousDamage = Character->DamageMultiplier;

	// Checks if the damage multiplier not already set
	// If the damage multiplier already set then don't pick up
	if(PreviousDamage * DamageMultiplier == DamageMultiplier)
	{
		IsPickedUp = true; // Stops the lifetime timer from destroying this
		AudioComponent->Play(); // Plays pick up audio
		PlayerCharacter->DamageMultiplier *= DamageMultiplier; // Sets the new damage

		// Starts the timer for how long the damage should be multiplied
		GetWorld()->GetTimerManager().SetTimer(PowerUpTimer, this, &ADamagePickUpActor::OnTimerFinished, PickUpTimer, false);

		// Hides and disables the pickup
		// Allows the pickup to run in the background
		MeshComponent->SetVisibility(false);
		PickUpCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	
}

void ADamagePickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	// Calls the pick up function in the base class which called the correct pick up above
	
}

void ADamagePickUpActor::OnTimerFinished()
{
	// Resets and destroys when the pickup time has finished
	PlayerCharacter->DamageMultiplier = PreviousDamage;
	Destroy();
}
