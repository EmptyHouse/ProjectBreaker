// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMesh.h"


#include "PlayerCompanion.generated.h"

UCLASS()
class PROJECTBREAKER_API APlayerCompanion : public APawn
{
	GENERATED_BODY()

public:
	APlayerCompanion();

private:
	/* The current speed of our companion character */
	UPROPERTY(BlueprintReadWrite, Category = "Movmement", meta=(AllowPrivateAccess="true"))
	FVector CurrentSpeed;

protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/* The  rate at which our companion will rotae */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float RotationRate;

	/* The Maximum speed at which our character will move */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MaxSpeed;

	/* The acceleration rate of our companion */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float AccelerationRate;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VisualMovement")
	float HeightOfHover;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VisualMovment")
	float RateOfHover;

	/* The mesh component of our companion */
	UPROPERTY(BlueprintReadWrite, Category = "Visual")
	UStaticMesh* AssociatedMesh;

};
