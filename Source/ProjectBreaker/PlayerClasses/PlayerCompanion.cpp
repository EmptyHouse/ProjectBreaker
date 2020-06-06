// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCompanion.h"

// Sets default values
APlayerCompanion::APlayerCompanion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RotationRate = 25.f;

}

// Called when the game starts or when spawned
void APlayerCompanion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

