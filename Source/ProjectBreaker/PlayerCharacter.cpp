// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region Initialization

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
	CurrentCameraFacingDirection = ESpriteCameraFacingDirection::ForwardFace;
	SetShouldUseCameraDirection(true);
	UpdatePlayerCameraFacingState();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePlayerCameraFacingState();
}

#pragma endregion

#pragma region PlayerInput

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(MoveRightInput, this, &APlayerCharacter::HandleHorizontalMovement);
	PlayerInputComponent->BindAxis(MoveForwardInput, this, &APlayerCharacter::HandleVerticalMovement);
	PlayerInputComponent->BindAxis(RotateCameraInput, this, &APlayerCharacter::HandleCameraRotation);
	PlayerInputComponent->BindAction(JumpInput, EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(JumpInput, EInputEvent::IE_Released, this, &APlayerCharacter::StopJumping);

}

void APlayerCharacter::HandleCameraRotation(float axis)
{
	AddControllerYawInput(-axis * CharacterRotationRate);
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

#pragma endregion

#pragma region PlayerCameraMethods

void APlayerCharacter::UpdatePlayerCameraFacingState()
{

	if (bShouldUseCameraDirection)
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

		const FVector CameraForwardVector = CameraManager->GetActorForwardVector().GetSafeNormal2D();
		const FVector ActorForwardVector = GetActorForwardVector().GetSafeNormal2D();
		const float SpriteCameraAngle = UKismetMathLibrary::DegAcos(FVector::DotProduct(CameraForwardVector, ActorForwardVector));
		const FVector CrossProduct = FVector::CrossProduct(CameraForwardVector, ActorForwardVector);

		FVector NewRotation = FVector::ForwardVector;

		const float FinalAngle = FMath::Sign(CrossProduct.Z) > 0 ? 360.f - SpriteCameraAngle : SpriteCameraAngle;
		UE_LOG(LogTemp, Warning, TEXT("Final angle: %f"), FinalAngle);

		if (FinalAngle >= 335.f || FinalAngle < 35.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::ForwardFace;
		}
		else if (FinalAngle >= 35.f && FinalAngle < 65.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::ForwardRight;
		}
		else if (FinalAngle >= 65.f && FinalAngle < 125.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::RightFace;
		}
		else if (FinalAngle >= 125.f && FinalAngle < 155.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::BackRight;
		}
		else if (FinalAngle >= 155.f && FinalAngle < 215.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::BackFace;
		}
		else if (FinalAngle >= 215.f && FinalAngle < 245.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::BackLeft;
		}
		else if (FinalAngle >= 245.f && FinalAngle < 305.f)
		{
			NewRotation = FVector(-5.f, 0.0f, 0.0f);
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::LeftFace;
		}
		else if (FinalAngle >= 305.f && FinalAngle < 335.f)
		{
			CurrentCameraFacingDirection = ESpriteCameraFacingDirection::ForwardLeft;
		}
	}

	if (DirectionToFlipbookMap.Contains(CurrentCameraFacingDirection))
	{
		UPaperFlipbook* FlipbookToUse = DirectionToFlipbookMap[CurrentCameraFacingDirection];
		GetSprite()->SetFlipbook(FlipbookToUse);
	}
}

void APlayerCharacter::SetShouldUseCameraDirection(bool bShouldUseCamera)
{
	bShouldUseCameraDirection = bShouldUseCamera;
}

void APlayerCharacter::SetSpriteFacingDirection(ESpriteCameraFacingDirection NewFacingDirection)
{
	CurrentCameraFacingDirection = NewFacingDirection;
}

#pragma endregion