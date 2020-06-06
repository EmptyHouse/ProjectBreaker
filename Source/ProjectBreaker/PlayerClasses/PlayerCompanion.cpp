// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCompanion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerCompanion::APlayerCompanion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetPlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

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

	UpdateCompanionHover(DeltaTime);

	UpdateCompanionRotation();

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

/* Updates the local position of our mesh component */
void APlayerCompanion::UpdateCompanionHover(float DeltaTime)
{
	CurrentHoverTimer += DeltaTime;
	if (CurrentHoverTimer > 1000.f) CurrentHoverTimer -= 1000;
	float OriginZ = GetActorLocation().Z;
	AssociatedMesh->SetWorldLocation(FVector(0, 0, OriginZ + UKismetMathLibrary::Sin(CurrentHoverTimer * RateOfHover) * HeightOfHover));
}

/* Updates the desired position of the companion based on the target player character */
void APlayerCompanion::UpdateCompanionPosition()
{

}


/* Updates the Companion Rotation based on the position of the player's aim offset */
void APlayerCompanion::UpdateCompanionRotation()
{

}

/* Gets the aim offset vector for our player.*/
FVector APlayerCompanion::GetAimOffset()
{
	return FVector();
}
#pragma endregion

