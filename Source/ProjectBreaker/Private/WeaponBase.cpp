// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "ThirdPersonCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->InitCapsuleSize(10.f, 10.f);
	SetRootComponent(CapsuleComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentHit.AddDynamic(this, &AWeaponBase::OnWeaponAttackHit);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponAttackOverlap);

	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetNotifyRigidBodyCollision(false);
	CapsuleComponent->SetGenerateOverlapEvents(false);
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
		//UE_LOG(LogTemp, Warning, TEXT("Starting attack"));
		const FName CollisionProfileName = bBelongsToPlayer ? FName(TEXT("PlayerWeapon_Active")) : FName(TEXT("EnemyWeapon_Active"));
		CapsuleComponent->SetCollisionProfileName(CollisionProfileName);
		CapsuleComponent->SetGenerateOverlapEvents(true);
		CapsuleComponent->SetNotifyRigidBodyCollision(true);
	}
}

void AWeaponBase::OnWeaponAttackEnd_Implementation()
{
	const FName InactiveWeaponProfile = FName(TEXT("NoCollision"));
	if (StaticMesh)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Ending attack"));
		CapsuleComponent->SetCollisionProfileName(InactiveWeaponProfile);
		CapsuleComponent->SetGenerateOverlapEvents(false);
		CapsuleComponent->SetNotifyRigidBodyCollision(false);
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

