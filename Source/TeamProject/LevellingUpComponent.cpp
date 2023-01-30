// Fill out your copyright notice in the Description page of Project Settings.


#include "LevellingUpComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
ULevellingUpComponent::ULevellingUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevellingUpComponent::BeginPlay()
{
	Super::BeginPlay();

	CalculatePercentage();
	// ...
	
}


// Called every frame
void ULevellingUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Set the currentXP to the linear interpolation between the CurrentXP and the BufferXP
	CurrentXP = FMath::FInterpTo(CurrentXP, BufferXP, DeltaTime, 5.0f);

	//Ceil the float to ensure that there are no floating points
	CurrentXP = FMath::CeilToFloat(CurrentXP);

	//Update the percentage for the progress bar
	CalculatePercentage();

	//Level up the player if the CurrentXP exceeds the requirements to level up
	if (CurrentXP >= CurrentMaxXP)
	{
		LevelUP();
	}
}

//Calculate the current XP percentage for the progress bars in the XP widgets 

void ULevellingUpComponent::CalculatePercentage()
{
	CurrentPercentage = CurrentXP / CurrentMaxXP;
}

//Calculate the MaxXP that the player can get to before levelling up
void ULevellingUpComponent::CalculateMaxXP()
{
	//Calculate the next Level up requirement amount through an exponential equation
	float tempXP = BaseMaxXP * FMath::Pow(1.5f, CurrentXPLevel);

	//Ceil the temporary value to ensure that there are no floating points 
	tempXP = ceilf(tempXP);

	//Set the MaxXp to this new temporary value
	CurrentMaxXP = tempXP;

}

//Give the player XP
void ULevellingUpComponent::AddXP(float XPToAdd)
{
	BufferXP += XPToAdd;
}
//Level up the player
void ULevellingUpComponent::LevelUP()
{
	if (CurrentXPLevel >= LevelCap)
	{
		CurrentXP = CurrentMaxXP;
		BufferXP = CurrentMaxXP;
	}
	else
	{
		++CurrentXPLevel;

		CalculateMaxXP();
		CurrentXP = 0;
		BufferXP = 0;
		AvailableSkillPoints++;
	}
}

