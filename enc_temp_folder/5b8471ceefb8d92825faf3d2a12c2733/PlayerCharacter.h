// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbook.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum ESpriteCameraFacingDirection {
	ForwardFace, // Camera is looking parallel to the sprite's facing arrow.
	ForwardRight,
	RightFace,  // Camera is looking parallel to a vector pointing to the right of the actor
	BackRight,
	BackFace,	// Camera is looking opposite the sprite's facing arrow
	BackLeft, 
	LeftFace, // Camera is looking parallel to a vector pointing to the left of the actor.
	ForwardLeft,
};

/**
 * 
 */
UCLASS()
class PROJECTBREAKER_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowProtectedAccess = "true"), Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowProtectedAccess = "true"), Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowProtectedAccess = "true"), Category = "Movement")
	float CharacterRotationRate = 2.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowProtectedAccess = "true"), Category = "Movement")
	float CharacterMovementRate = 1.0f;

#pragma region Camera Movement Methods

public:

	/** Sets the character to use the camera's facing direction for its viewing sprites */
	void SetShouldUseCameraDirection(bool bShouldUseCamera);

	/** Sets the sprite facing direction of this actor */
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
	void SetSpriteFacingDirection(ESpriteCameraFacingDirection NewFacingDirection);

protected:
	
	/** Map relating the camera facing direction with the relevant flipbook animation play. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowProtectedAccess = "true"), Category = "Camera Movement")
	TMap<TEnumAsByte<ESpriteCameraFacingDirection>, UPaperFlipbook*> DirectionToFlipbookMap;

private:

	/** Current direction the camera is facing relative to the sprite's forward vector */
	UPROPERTY(VisibleAnywhere, Category = "Camera Movement")
	TEnumAsByte<ESpriteCameraFacingDirection> CurrentCameraFacingDirection;
	
	/** Returns the current camera view relative to the forward vector of the sprite */
	UFUNCTION(BlueprintPure)
	ESpriteCameraFacingDirection GetCharacterCameraFacingDirection() const {
		return CurrentCameraFacingDirection;
	}

	/** If true, the sprite will rotate depending on the camera's angle relative to the forward vector */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera Movement")
	bool bShouldUseCameraDirection;

	/** Update the sprite facing direction */
	UFUNCTION(BlueprintCallable, Category = "Camera Movement")
	void UpdatePlayerCameraFacingState();

#pragma endregion

private:

	/** Rotate the camera using mouse input */
	UFUNCTION()
	void HandleCameraRotation(float axis);
	
	/** Move sprite actor in the direction the camera is facing */
	UFUNCTION()
	void HandleVerticalMovement(float axis);

	/** Move sprite to laterally */
	UFUNCTION()
	void HandleHorizontalMovement(float axis);

};
