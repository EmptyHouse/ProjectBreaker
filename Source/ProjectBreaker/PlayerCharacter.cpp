// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	// Use the controller yaw (z-axis) rotation, but allow for independent pitch and roll.
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::HandleHorizontalMovement);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerCharacter::HandleVerticalMovement);
	PlayerInputComponent->BindAxis("RotateCamera", this, &APlayerCharacter::HandleCameraRotation);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APlayerCharacter::StopJumping);

}

void APlayerCharacter::HandleCameraRotation(float axis)
{
	AddControllerYawInput(axis * CharacterRotationRate);
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
	AddMovementInput(MoveDirection);
}