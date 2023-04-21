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
	
	if(PreviousDamage * DamageMultiplier == DamageMultiplier)
	{
		IsPickedUp = true;
		AudioComponent->Play();
		PlayerCharacter->DamageMultiplier *= DamageMultiplier;
		GetWorld()->GetTimerManager().SetTimer(PowerUpTimer, this, &ADamagePickUpActor::OnTimerFinished, PickUpTimer, false);

		// Hides and disables the pickup
		MeshComponent->SetVisibility(false);
		PickUpCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	
}

void ADamagePickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void ADamagePickUpActor::OnTimerFinished()
{
	PlayerCharacter->DamageMultiplier = PreviousDamage;
	Destroy();
}
