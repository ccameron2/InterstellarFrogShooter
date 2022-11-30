// Copyright Epic Games, Inc. All Rights Reserved.
#include "TeamProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ATeamProjectGameModeBase::ATeamProjectGameModeBase()
{
	
}

void ATeamProjectGameModeBase::BeginPlay()
{
	PlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerPawn = PlayerController->GetPawn();

	if (LandClass)
	{
		FTransform Transform;
		LandActor = GetWorld()->SpawnActorDeferred<ALand>(LandClass, Transform);
	}

}

void ATeamProjectGameModeBase::OnGuiSetValues()
{
	int seed = 89514156;
	int type = 0;
	LandActor->Init(seed,type);

	FTransform Transform;
	FVector Location = { 0,0,0 };
	Transform.SetTranslation(Location);

	UGameplayStatics::FinishSpawningActor(LandActor, Transform);

	if (PlayerCharacterClass)
	{
		Location = { 0,0,1000 };
		Transform.SetTranslation(Location);

		APlayerCharacter* PlayerActor = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacterClass, Transform);
		PlayerController->UnPossess();
		PlayerController->AutoReceiveInput = EAutoReceiveInput::Player0;
		PlayerController->Possess((PlayerActor));

		PlayerPawn->Destroy();
		PlayerPawn = PlayerActor;

		PlayerController->RebindCharacter(PlayerActor);
	}

}