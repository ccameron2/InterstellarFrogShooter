// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUpActor.h"

#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"

AHealthPickUpActor::AHealthPickUpActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PickUpType = EPickUpType::Health;
}

void AHealthPickUpActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	

	if(IsPickedUp && !AudioComponent->IsPlaying())
	{
		Destroy();
	}
}

void AHealthPickUpActor::OnPickUp(APlayerCharacter* Character)
{
	Super::OnPickUp(Character);

	// Checks if the health if smaller than max health
	// if not don't pick up
	if(Character->PlayerHealth < Character->PlayerMaxHealth)
	{
		IsPickedUp = true;
		AudioComponent->Play(); 
		
		// Calculate if the health amount is bigger than max health or not
		int newHealth = Character->PlayerHealth + HealthAmount;
		if(newHealth >= Character->PlayerMaxHealth)
		{
			int diff = newHealth - Character->PlayerMaxHealth;
			newHealth -= diff;

			Character->PlayerHealth = newHealth;
		}
		else
		{
			Character->PlayerHealth += HealthAmount;
		}
		
		// Hides and disables the pickup
		// Allows the pickup to run in the background
		MeshComponent->SetVisibility(false);
		PickUpCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	
}

void AHealthPickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	
}
