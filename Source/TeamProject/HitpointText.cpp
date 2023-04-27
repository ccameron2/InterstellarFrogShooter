//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.


#include "HitpointText.h"

// Sets default values
AHitpointText::AHitpointText()
{
	HitpointsText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Hitpoint Text Box"));
	HitpointsText->SetupAttachment(RootComponent);
	SetRootComponent(HitpointsText);
}

// Called when the game starts or when spawned
void AHitpointText::BeginPlay()
{
	Super::BeginPlay();
}

