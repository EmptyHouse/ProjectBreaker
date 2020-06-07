// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCompanion.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h"


// Sets default values
APlayerCompanion::APlayerCompanion()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = CapsuleCollider;

	AssociatedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompanionMesh"));
	AssociatedMesh->SetupAttachment(RootComponent);

	bIsShooting = false;

}

// Called when the game starts or when spawned
void APlayerCompanion::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayerCharacter = Cast<AThirdPersonCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ensure(TargetPlayerCharacter);
	ensure(ProjectileToLaunch);
}

// Called every frame
void APlayerCompanion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCompanionPosition(DeltaTime);
	UpdateCompanionRotation();

	UpdateCompanionHover(DeltaTime);

	if (bIsShooting)
	{
		TimeRemainingUntilNextShot -= DeltaTime;
		SpawnNewProjectileIfReady();
	}

}

// Called to bind functionality to input
void APlayerCompanion::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(INPUT_COMPANION_FIRE, EInputEvent::IE_Pressed, this, &APlayerCompanion::FireWeaponPressed);
	PlayerInputComponent->BindAction(INPUT_COMPANION_FIRE, EInputEvent::IE_Released, this, &APlayerCompanion::FireWeaponReleased);

	

}

#pragma region combat functions
/* The fire weapon button was pressed */
void APlayerCompanion::FireWeaponPressed()
{
	bIsShooting = true;
	TimeRemainingUntilNextShot = 0;
	SpawnNewProjectileIfReady();
	UE_LOG(LogTemp, Warning, TEXT("Press"));
}

/* The fire weapon button was released */
void APlayerCompanion::FireWeaponReleased()
{
	bIsShooting = false;
	UE_LOG(LogTemp, Warning, TEXT("Release"));
}

/* Spawns a new  */
void APlayerCompanion::SpawnNewProjectileIfReady()
{
	if (TimeRemainingUntilNextShot > 0) { return; }
	TimeRemainingUntilNextShot += TIME_BETWEEN_SHOTS;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = FName(TEXT("Bullet"));
	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileToLaunch.Get(), GetActorLocation() + GetActorForwardVector() * 25, GetActorRotation(), SpawnParameters);
	UE_LOG(LogTemp, Warning, TEXT("SPANWED"));
}
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
	FHitResult OutHit;
	FVector Origin;
	FVector CameraDirection;
	APlayerController* PlayerController = Cast<APlayerController>(TargetPlayerCharacter->Controller);
	if (!PlayerController || !PlayerController->DeprojectMousePositionToWorld(Origin, CameraDirection))
	{
		return;
	}


	FVector End = Origin + CameraDirection * 100000.f;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionQueryParams Params;
	Params.bDebugQuery = true;
	FCollisionResponseParams ResponseParam;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Origin, End, TraceChannel, Params, ResponseParam))
	{
		FVector PointToLookAt = OutHit.ImpactPoint - GetActorLocation();
		PointToLookAt.Z = 0;
		PointToLookAt.Normalize();
		float DesiredYawRotation = FMath::RadiansToDegrees(FMath::Atan2(PointToLookAt.Y, PointToLookAt.X));
		
		SetActorRotation(FMath::Lerp(GetActorRotation(), FRotator(0, DesiredYawRotation, 0), .1f));
	}
}
#pragma endregion

