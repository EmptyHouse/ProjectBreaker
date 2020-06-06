// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCompanion.h"

// Sets default values
APlayerCompanion::APlayerCompanion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = CapsuleCollider;

	AssociatedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompanionMesh"));
	AssociatedMesh->AttachTo(RootComponent);

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

	UpdateCompanionHover();

}

// Called to bind functionality to input
void APlayerCompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma region getter/setters

/* Returns the current speed of our companion */
FVector APlayerCompanion::GetSpeed()
{
	return CurrentSpeed;
}

#pragma endregion

#pragma region movement

void APlayerCompanion::UpdateCompanionHover()
{

}

void APlayerCompanion::UpdateCompanionPosition()
{

}

void APlayerCompanion::UpdateCompanionRotation()
{

}

#pragma endregion

