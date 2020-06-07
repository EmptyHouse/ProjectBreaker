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
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeaponBase::AttachToCharacter(AThirdPersonCharacter* NewWeaponOwner, const FName& AttachSocketName)
{
	if (NewWeaponOwner)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
		AttachToComponent(NewWeaponOwner->GetMesh(),AttachmentRules, AttachSocketName);
		SetOwner(NewWeaponOwner);
		return true;
	}
	return false;
}

