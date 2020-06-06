// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeLocation(FVector::ZeroVector);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	FollowCamera->AttachTo(SpringArm);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerCharacter::HandleHorizontalMovement);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerCharacter::HandleVerticalMovement);
}

void APlayerCharacter::HandleVerticalMovement(float axis)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector MoveDirection = CameraManager->GetActorForwardVector() * axis;
	AddMovementInput(MoveDirection);

}

void APlayerCharacter::HandleHorizontalMovement(float axis)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector MoveDirection = CameraManager->GetActorRightVector() * axis;
}