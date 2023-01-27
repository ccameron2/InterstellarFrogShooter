//Jonathan
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextRenderComponent.h"

#include "GameFramework/Actor.h"
#include "HitpointText.generated.h"



UCLASS()
class TEAMPROJECT_API AHitpointText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitpointText();


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTextRenderComponent* HitpointsText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
