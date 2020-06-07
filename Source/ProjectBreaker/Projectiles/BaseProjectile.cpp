// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);

}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	PreviousPosition = GetActorLocation();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PreviousPosition = GetActorLocation();
	DidWeCollideWithAnything();
}


// Checks to see if we collide with anything using a sphere trace
bool ABaseProjectile::DidWeCollideWithAnything()
{
	FName EnemyProfile = FName(TEXT("Enemy"));
	FHitResult OutHit;
	TArray<AActor*> IgnoredActors;
	if (UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), PreviousPosition, GetActorLocation(), SPHERE_RADIUS,
	EnemyProfile, false, IgnoredActors, EDrawDebugTrace::ForDuration, OutHit, true, FColor(255, 0, 0), FColor(0, 255, 0), 3))
	{
	
		return true;
	}
	return false;
}
