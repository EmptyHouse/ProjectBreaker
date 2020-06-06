// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowProtectedAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowProtectedAccess = "true"))
	UCameraComponent* FollowCamera;

private:
	
	UFUNCTION()
	void HandleVerticalMovement(float axis);

	UFUNCTION()
	void HandleHorizontalMovement(float axis);

};
