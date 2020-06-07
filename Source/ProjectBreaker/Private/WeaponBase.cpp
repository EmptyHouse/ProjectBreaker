// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "ThirdPersonCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentHit.AddDynamic(this, &AWeaponBase::OnWeaponAttackHit);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponAttackOverlap);

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetNotifyRigidBodyCollision(false);
	StaticMesh->SetGenerateOverlapEvents(false);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeaponBase::AttachToCharacter(AThirdPersonCharacter* NewWeaponOwner, const FName& AttachSocketName, bool bIsPlayerWeapon)
{
	if (NewWeaponOwner)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		AttachToComponent(NewWeaponOwner->GetMesh(),AttachmentRules, AttachSocketName);
		SetOwner(NewWeaponOwner);
		bBelongsToPlayer = bIsPlayerWeapon;
		return true;
	}
	return false;
}

void AWeaponBase::OnWeaponAttackStart_Implementation()
{
	if (StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting attack"));
		const FName CollisionProfileName = bBelongsToPlayer ? FName(TEXT("PlayerWeapon_Active")) : FName(TEXT("EnemyWeapon_Active"));
		StaticMesh->SetCollisionProfileName(CollisionProfileName);
		StaticMesh->SetNotifyRigidBodyCollision(true);
	}
}

void AWeaponBase::OnWeaponAttackEnd_Implementation()
{
	const FName InactiveWeaponProfile = FName(TEXT("NoCollision"));
	if (StaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ending attack"));
		StaticMesh->SetCollisionProfileName(InactiveWeaponProfile);
		StaticMesh->SetNotifyRigidBodyCollision(false);
	}
}

void AWeaponBase::OnWeaponAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("On Weapon Attack Hit. Actor Hit: %s"), *OtherActor->GetName());
	OnWeaponAttackSuccessful(OtherActor, Hit);
}

void AWeaponBase::OnWeaponAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Weapon Attack Overlap. Actor Hit: %s"), *OtherActor->GetName());
}

void AWeaponBase::OnWeaponAttackSuccessful_Implementation(AActor* OtherActor, const FHitResult& HitResult)
{

}

