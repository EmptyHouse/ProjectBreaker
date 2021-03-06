// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectiles/BaseProjectile.h"

#include "PlayerCompanion.generated.h"

UCLASS()
class PROJECTBREAKER_API APlayerCompanion : public APawn
{
	GENERATED_BODY()

public:
	APlayerCompanion();

private:
	
	const float TIME_BETWEEN_SHOTS = .15F;

	/* Bool indicates whether our companion should be shooting */
	bool bIsShooting;

	/* The time remaining until the next shot can be fired. If this value is <= 0 you can shoot */
	float TimeRemainingUntilNextShot = 0;


	/* The current time for our hover property */
	float CurrentHoverTimer;

	int ProjectileCount = 0;

	/*  */
	void SpawnNewProjectileIfReady();
	

protected:
	virtual void BeginPlay() override;

	/* Updates the mesh component offset based on the hover values. (Primarily for visual purposes)*/
	virtual void UpdateCompanionHover(float DeltaTime);

	

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* The fire weapon was pressed by our player */
	void FireWeaponPressed();

	/* the fire weapon was released by our player */
	void FireWeaponReleased();

public:
	/* The  rate at which our companion will rotae */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float RotationRate = 25.f;

	/* The Maximum speed at which our character will move */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MaxSpeed = 100.f;

	/* The acceleration rate of our companion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float AccelerationRate = 100.f;

	/* Height of our Hover */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VisualMovement")
	float HeightOfHover = 20.f;

	/* The rate at which our companion will hover */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VisualMovement")
	float RateOfHover = 25.f;

	/* Capsule Collider Component. Will be RootComponent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
	UCapsuleComponent* CapsuleCollider;

	/* The mesh component of our companion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
	UStaticMeshComponent* AssociatedMesh;

	/* The offset from the player character's position that we will follow. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VisualMovement")
	FVector FollowPlayerOffset = FVector(0, 0, 15);

	/* The buffer distance that we will give our player we don't want to follow too closely to them */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VisualMovement")
	float FollowRadiusFromPlayer = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseProjectile> ProjectileToLaunch;

	/*Updates the companions position based on the position of the player */
	virtual void UpdateCompanionPosition(float DeltaTime, AActor* TargetToFollow);
	
	/* Updates the rotation based on the player's aim offset */
	virtual void UpdateCompanionRotation(APlayerController* PlayerController);
};
