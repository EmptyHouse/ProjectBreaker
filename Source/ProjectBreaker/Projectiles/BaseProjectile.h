// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

#include "BaseProjectile.generated.h"

UCLASS()
class PROJECTBREAKER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	/* The associated projectile movement component */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement", meta=(AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/* The sphere component that will represent our projectile*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
	UStaticMeshComponent* StaticMesh;

private:

	const float SPHERE_RADIUS = 50.f;

	/* A reference to the previous position of our projectile component */
	FVector PreviousPosition;

	float DespawnAfterTime = 10.f;

private:
	/* Runs a check to see if we have collided with anything */
	virtual bool DidWeCollideWithAnything();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
