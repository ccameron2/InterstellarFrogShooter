// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUpActor.h"

#include "PlayerCharacter.h"

AHealthPickUpActor::AHealthPickUpActor()
{
	PickUpType = EPickUpType::Health;
}

void AHealthPickUpActor::OnPickUp(APlayerCharacter* Character)
{
	Super::OnPickUp(Character);
	UE_LOG(LogTemp, Warning, TEXT("Picked Up"));
	if(Character->PlayerHealth < Character->PlayerMaxHealth)
	{
		// Calculate if the health amount is bigger than max health or not
		int newHealth = Character->PlayerHealth + HealthAmount;
		if(newHealth >= Character->PlayerMaxHealth)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Health %f"), Character->PlayerHealth);
			UE_LOG(LogTemp, Warning, TEXT("Health before calulation %d"), newHealth);
			int diff = newHealth - Character->PlayerMaxHealth;
			newHealth -= diff;
			UE_LOG(LogTemp, Warning, TEXT("Health after calulation %d"), newHealth);
			Character->PlayerHealth = newHealth;
		}
		else
		{
			Character->PlayerHealth += HealthAmount;
		}
		
		Destroy();
	}

}

void AHealthPickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	
}
