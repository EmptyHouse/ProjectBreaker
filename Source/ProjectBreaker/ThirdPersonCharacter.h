// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.h"
#include "ThirdPersonHUD.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "WeaponBase.h"
#include "PlayerClasses/PlayerCompanion.h"
#include "ThirdPersonCharacter.generated.h"

#pragma region Structs_And_Enums


// Structs aren't handled by garbage collection. So encapsulate/instantiate as a part of a UObject.
// UObject - Generic UE4 Container

// Inherit from FTableRowBase to create Data Table entries.
// Blueprint Type Decorator - Usable in UE4 Blueprint

USTRUCT(BlueprintType)
struct FPlayerAttackMontage : public FTableRowBase
{
	GENERATED_BODY()

	/** Melee Fist Attack Montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage;

	/** amount of start sections within our montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AnimSectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;
};

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Disabled;

	// default constructor
	FMeleeCollisionProfile()
	{
		Enabled = FName(TEXT("Weapon"));
		Disabled = FName(TEXT("NoCollision"));
	}
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MELEE_FIST			UMETA(DisplayName = "Melee - Fist"),
	MELEE_KICK			UMETA(DisplayName = "Melee - Kick")
};

UENUM(BlueprintType)
enum class ELogLevel : uint8 {
	TRACE			UMETA(DisplayName = "Trace"),
	DEBUG			UMETA(DisplayName = "Debug"),
	INFO			UMETA(DisplayName = "Info"),
	WARNING			UMETA(DisplayName = "Warning"),
	ERROR			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput : uint8 {
	ALL				UMETA(DisplayName = "All levels"),
	OUTPUT_LOG		UMETA(DisplayName = "Output log"),
	SCREEN			UMETA(DisplayName = "Screen")
};

#pragma endregion Structs_And_Enums

UCLASS(config=Game)
class PROJECTBREAKER_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	bool bCanUseFreeLook;

	/* Melee Data Table*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UDataTable* MeleeAttackDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* PunchSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftMeleeCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightMeleeCollisionBox;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* CharacterAudioComponent;

	FMeleeCollisionProfile CollisionProfile;

	bool IsAnimationBlended;

	bool IsKeyboardEnabled;

	EAttackType CurrentAttackType;

	FVector LockOnTarget;

	/* The associated player companion */
	APlayerCompanion* AssignedPlayerCompanion;
		
public:
	AThirdPersonCharacter();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Companion")
	TSubclassOf<APlayerCompanion> PlayerCompanionToSpawn;
	

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void Tick(float DeltaTime) override;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetIsKeyboardEnabled(bool isEnabled)
	{
		IsKeyboardEnabled = isEnabled;
	}

	UFUNCTION(BlueprintPure)
	EAttackType GetCurrentAttackType()
	{
		return CurrentAttackType;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowProtectedAcccess = "true"))
	UAnimMontage* AttackMontage;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Methods tied to delegates need to be marked with the UFUNCTION decorator
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Triggered when collider overlaps another component
	UFUNCTION()
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Triggered when collider exits overlap with component
	UFUNCTION()
	void OnAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EndLockOn();

	UFUNCTION(BlueprintCallable)
	void BeginLockOn(ABaseEnemy* enemyToTarget);

	void CompanionFirePressed();

	void CompanionFireReleased();

protected:

	/** Class of default weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> DefaultWeaponClass;

	/** Currently equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = true), Category = "Weapon")
	AWeaponBase* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowProtectedAccess = true), Category = "Lock-On")
	FVector LockOnOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On", meta=(ClampMin="0.001", ClampMax = "1.0"))
	float LockOnRotationLerp;

	ABaseEnemy* EnemyLockedOnTo;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On")
	FTimeline CameraLockOnTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On")
	UCurveFloat* LockOnCurve;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	bool IsCharacterLockedOn();

	UFUNCTION()
	void PunchAttack();

	UFUNCTION()
	void KickAttack();

	void AttackInput(EAttackType AttackType);

	UFUNCTION()
	void AttackStart();

	UFUNCTION()
	void AttackEnd();

	UFUNCTION(BlueprintPure, Category = Animation)
	bool GetIsAnimationBlended() const {
		return IsAnimationBlended;
	}

	UFUNCTION()
	void OnLockOnCameraMoveFinished();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(AWeaponBase* Weapon);

private:

	void RotateSideways(float value);

	void RotateUpwards(float value);

	void OnLockOnPressed();

	void RotateToTargetEnemy();

	void AttachSocketCollisionBoxes(EAttackType attackType);

	UFUNCTION()
	void MoveLockOnCamera();

	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	void Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput = ELogOutput::ALL);
};

