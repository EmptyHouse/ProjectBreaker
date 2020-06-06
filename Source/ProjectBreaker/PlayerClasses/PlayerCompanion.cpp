// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCompanion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathVectorCommon.h"


// Sets default values
APlayerCompanion::APlayerCompanion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = CapsuleCollider;

	AssociatedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompanionMesh"));
	AssociatedMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlayerCompanion::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

// Called every frame
void APlayerCompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCompanionPosition(DeltaTime);
	UpdateCompanionRotation();

	UpdateCompanionHover(DeltaTime);


}

// Called to bind functionality to input
void APlayerCompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma region getter/setters

#pragma endregion

#pragma region movement

/* Updates the local position of our mesh component */
void APlayerCompanion::UpdateCompanionHover(float DeltaTime)
{
	CurrentHoverTimer += DeltaTime;
	if (CurrentHoverTimer > 1000.f) CurrentHoverTimer -= 1000;
	FVector OriginLocation = GetActorLocation();
	AssociatedMesh->SetWorldLocation(FVector(OriginLocation.X, OriginLocation.Y, OriginLocation.Z + UKismetMathLibrary::Sin(CurrentHoverTimer * RateOfHover) * HeightOfHover));
}

/* Updates the desired position of the companion based on the target player character */
void APlayerCompanion::UpdateCompanionPosition(float DeltaTime)
{
	FVector FollowPosition = TargetPlayerCharacter->GetActorLocation() + FollowPlayerOffset;
	FVector RadiusOffset = (GetActorLocation() - TargetPlayerCharacter->GetActorLocation());
	RadiusOffset.Z = 0;
	RadiusOffset.Normalize();

	RadiusOffset *= FollowRadiusFromPlayer;

	FollowPosition += RadiusOffset;//our goal follow point
	FVector AdjustedGoalLocation = FMath::Lerp<FVector>(GetActorLocation(), FollowPosition, 5 * DeltaTime);

	SetActorLocation(AdjustedGoalLocation);
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

