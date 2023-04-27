// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUpActor.h"

#include "HitpointText.h"

#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UENUM()
enum class EAIState : uint8
{
	Decision = 0, // Default State
	Patrol, 
	FindCover,
	Run,
	Chase,
	Shoot
};

UENUM()
enum class EDecisionReasons
{
	None = 0,
	CoverNotFound,
	BeingShot,
	PlayerNotFound,

	Clear
};

USTRUCT(BlueprintType)
struct FDrops
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int Rate;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABasePickUpActor> PickupClass;
};

UCLASS()
class TEAMPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Shoot(AActor* TargetActor);
	
private:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void SpawnDrop();

	void CalculateDrop();
	
public:
	UPROPERTY(EditAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		EAIState State;

	UPROPERTY(EditAnywhere)
		EDecisionReasons Reasons;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float Damage = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float RunAwayDistance = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MinXPAmount = 10.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MaxXPAmount = 15.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MaxWorldX = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MinWorldX = -5000.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MaxWorldY = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float MinWorldY = -5000.0f;

	UPROPERTY(EditAnywhere, Category = "AI Settings")
		float ShootTimer = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Score")
		float ScoreMultiplier = 2.0f;
	
	UPROPERTY()
		FVector ShootFromLocation; 

private:
	UPROPERTY(EditAnywhere)
		class UNavigationInvokerComponent* NavInvoker;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* FrogMesh;
	
	UPROPERTY()
		UMaterialInstanceDynamic* BodyInstanceMaterial;

	UPROPERTY()
		UMaterialInstanceDynamic* ToesAndBellyInstanceMaterial;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AHitpointText> HitPointText;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.0f;

	UPROPERTY()
		float XPAmount = 0.0f;
	
	// Pick up classes can be added to the map with there pick up type
	// These can be added in the AI blueprints
	// The drop rate can also be selected in the blueprint
	// This allows different AI to have different pickups depending on their settings
	UPROPERTY(EditAnywhere, Category="Drops")
		TMap<EPickUpType, FDrops> PickupMap;	
	
	int PickupTotalProbability = 0;

	EPickUpType SelectedType;

	//Allows the setting up of different Frog Colours through the AI Blueprints
	UPROPERTY(EditAnywhere, Category="Enemy Type")
		TArray<FLinearColor> BodyColourArray;
	
	UPROPERTY(EditAnywhere, Category="Enemy Type")
		TArray<FLinearColor> ToesAndBellyColourArray;

	//Effects for each Frog type that is able to be fine-tuned in the AI Blueprints
	UPROPERTY(EditAnywhere, Category="Enemy Type")
		float DamageTypeMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category="Enemy Type")
		float HealthTypeMultiplier = 1.2f;

	UPROPERTY(EditAnywhere, Category="Enemy Type")
		float SpeedTypeMultiplier = 1.5f;
		
};
