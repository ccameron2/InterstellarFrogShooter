// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedPickUpActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"

ASpeedPickUpActor::ASpeedPickUpActor()
{
	PickUpType = EPickUpType::Damage;
}

void ASpeedPickUpActor::OnPickUp(APlayerCharacter* Character)
{
	Super::OnPickUp(Character);

	PlayerCharacter = Character;
	PreviousSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

	if(PreviousSpeed != Character->DefaultMaxSpeed * SpeedMultiplier)
	{
		IsPickedUp = true;
		AudioComponent->Play();
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiplier;
		GetWorld()->GetTimerManager().SetTimer(SpeedTimer, this, &ASpeedPickUpActor::OnTimerFinished, PickUpTimer, false);

		// Hides and disables the pickup
		MeshComponent->SetVisibility(false);
		PickUpCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASpeedPickUpActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASpeedPickUpActor::OnTimerFinished()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PreviousSpeed;
	Destroy();
}
