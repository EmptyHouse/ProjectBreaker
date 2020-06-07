

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "ThirdPersonHUD.h"
#include "Engine.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AThirdPersonProjectCharacter

AThirdPersonCharacter::AThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	bCanUseFreeLook = true;
	PrimaryActorTick.bCanEverTick = true;

	// Load Player Attack Montage Data Table
	static ConstructorHelpers::FObjectFinder<UDataTable> attackDataTable(TEXT("DataTable'/Game/DataTables/AttackDataTable.AttackDataTable'"));
	if (attackDataTable.Succeeded())
	{
		MeleeAttackDataTable = attackDataTable.Object;

		// Add dummy row to data table
		FPlayerAttackMontage attackMontage;
		attackMontage.Montage = NULL;
		attackMontage.AnimSectionCount = 0;
		attackMontage.Description = FString(TEXT("Created in ThirdPersonProjectCharacter constructor"));

		MeleeAttackDataTable->AddRow(FName(TEXT("Dummy")), attackMontage);
	}

	// Audio
	static ConstructorHelpers::FObjectFinder<USoundCue> punchSoundCue(TEXT("SoundCue'/Game/Audio/PunchSoundCue.PunchSoundCue'"));
	if (punchSoundCue.Succeeded())
	{
		PunchSoundCue = PunchSoundCue;
	}

	CharacterAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchAudioComponent"));
	CharacterAudioComponent->SetupAttachment(RootComponent);

	LeftMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFist"));
	LeftMeleeCollisionBox->SetupAttachment(RootComponent);
	LeftMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Disabled);
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	LeftMeleeCollisionBox->SetGenerateOverlapEvents(true);

	RightMeleeCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFist"));
	RightMeleeCollisionBox->SetupAttachment(RootComponent);
	RightMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Disabled);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	RightMeleeCollisionBox->SetGenerateOverlapEvents(true);
}

void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn player companion and setup
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = FName(TEXT("PlayerCompanion"));
	SpawnParameters.bNoFail = true;
	AssignedPlayerCompanion = GetWorld()->SpawnActor<APlayerCompanion>(PlayerCompanionToSpawn.Get(), GetActorLocation() + GetActorRightVector() * 50, GetActorRotation(), SpawnParameters);
	


	// Blueprint construction script may override delegate attachments. So instead of attaching components and delegates in constructor, attach in Begin Play.
	// Set OnHit event delegates
	LeftMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AThirdPersonCharacter::OnAttackHit);
	RightMeleeCollisionBox->OnComponentHit.AddDynamic(this, &AThirdPersonCharacter::OnAttackHit);

	LeftMeleeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnAttackBeginOverlap);
	RightMeleeCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AThirdPersonCharacter::OnAttackBeginOverlap);

	if (PunchSoundCue && CharacterAudioComponent)
	{
		CharacterAudioComponent->SetSound(PunchSoundCue);
	}

	if (LockOnCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineCallback.BindUFunction(this, FName{ ("MoveLockOnCamera") });
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("OnLockOnCameraMoveFinished") });

		CameraLockOnTimeline = FTimeline();
		CameraLockOnTimeline.AddInterpFloat(LockOnCurve, TimelineCallback);
		CameraLockOnTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}


	if (DefaultWeaponClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		AWeaponBase* DefaultWeapon = GetWorld()->SpawnActor<AWeaponBase>(DefaultWeaponClass.Get(), SpawnLocation, SpawnRotation, SpawnParams);
		if (DefaultWeapon)
		{
			EquipWeapon(DefaultWeapon);
		}

	}
	IsKeyboardEnabled = true;
	MovementSpeedMultiplier = 1.f;
	SprintKeyTimeHeld = 0.f;
	bIsSprintKeyHeld = false;
}

void AThirdPersonCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (IsCharacterLockedOn())
	{
		RotateToTargetEnemy();
	}

	if (CameraLockOnTimeline.IsPlaying())
	{
		CameraLockOnTimeline.TickTimeline(deltaTime);
	}

	UpdateCharacterMovement(deltaTime);

	//Companion
	AssignedPlayerCompanion->UpdateCompanionPosition(deltaTime, this);
	AssignedPlayerCompanion->UpdateCompanionRotation(Cast<APlayerController>(Controller));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonCharacter::MoveRight);
	PlayerInputComponent->BindAction("MovementAction", IE_Pressed, this, &AThirdPersonCharacter::HandleMovementKeyPressed);
	PlayerInputComponent->BindAction("MovementAction", IE_Released, this, &AThirdPersonCharacter::HandleMovementKeyReleased);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AThirdPersonCharacter::RotateSideways);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThirdPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AThirdPersonCharacter::RotateUpwards);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThirdPersonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AThirdPersonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AThirdPersonCharacter::TouchStopped);

	/*
	// Lock-on methods
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AThirdPersonCharacter::OnLockOnPressed);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &AThirdPersonCharacter::EndLockOn);
	*/

	// Attack methods
	PlayerInputComponent->BindAction("PlayerAttack", IE_Pressed, this, &AThirdPersonCharacter::PunchAttack);
	//PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AThirdPersonCharacter::KickAttack);

	
	//Companion Inputs
	PlayerInputComponent->BindAction(TEXT("CompanionFire"), IE_Pressed, this, &AThirdPersonCharacter::CompanionFirePressed);
	PlayerInputComponent->BindAction(TEXT("CompanionFire"), IE_Released, this, &AThirdPersonCharacter::CompanionFireReleased);
}

void AThirdPersonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AThirdPersonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AThirdPersonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (bCanUseFreeLook)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AThirdPersonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (bCanUseFreeLook)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AThirdPersonCharacter::MoveForward(float Value) {

	if ((Controller != NULL) && (Value != 0.0f) && IsKeyboardEnabled)
	{

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsKeyboardEnabled)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AThirdPersonCharacter::HandleMovementKeyPressed()
{
	bIsSprintKeyHeld = true;
}

void AThirdPersonCharacter::HandleMovementKeyReleased()
{
	bIsSprintKeyHeld = false;
	if (SprintKeyTimeHeld < KeyHoldTimeToSprint)
	{
		// Combat roll or other movement
	}
}

void AThirdPersonCharacter::UpdateCharacterMovement(float DeltaTime)
{

	bool bShouldUpdateMovementSpeedMultiplier = SprintKeyTimeHeld > KeyHoldTimeToSprint || MovementSpeedMultiplier > 0.f;
	SprintKeyTimeHeld = bIsSprintKeyHeld ? SprintKeyTimeHeld + DeltaTime : 0.f;
	
	if (bShouldUpdateMovementSpeedMultiplier)
	{
		float LerpB = bIsSprintKeyHeld ? MaxRunSpeedMultiplier : 0.f;
		MovementSpeedMultiplier = FMath::Lerp(MovementSpeedMultiplier, LerpB, RunSpeedLerpValue);
	}
	GetCharacterMovement()->MaxWalkSpeed = 600.f + (MovementSpeedMultiplier * MovementSpeedIncrementValue);
}

void AThirdPersonCharacter::RotateSideways(float value)
{
	if (bCanUseFreeLook)
	{
		AddControllerYawInput(value);
	}
}

void AThirdPersonCharacter::RotateUpwards(float value)
{
	if (bCanUseFreeLook)
	{
		AddControllerPitchInput(value);
	}
}

void AThirdPersonCharacter::EquipWeapon(AWeaponBase* Weapon)
{
	if (Weapon && Weapon->AttachToCharacter(this, FName(TEXT("Weapon_Socket_R")), true))
	{
		EquippedWeapon = Weapon;
	}
}

void AThirdPersonCharacter::OnLockOnPressed()
{
	TArray<AActor*> enemiesInWorld;
	TSubclassOf<ABaseEnemy> classToFind = ABaseEnemy::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, enemiesInWorld);
	if (enemiesInWorld.Num() > 0)
	{
		// Take the first enemy for now.
		ABaseEnemy* enemy = Cast<ABaseEnemy>(enemiesInWorld[0]);
		if (enemy)
		{
			BeginLockOn(enemy);
		}
	}
}

void AThirdPersonCharacter::BeginLockOn(ABaseEnemy* enemyToTarget)
{
	EnemyLockedOnTo = enemyToTarget;
	bCanUseFreeLook = false;
	LockOnTarget = LockOnOffset;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	CameraLockOnTimeline.Play();
}

bool AThirdPersonCharacter::IsCharacterLockedOn()
{
	return EnemyLockedOnTo != NULL;
}

void AThirdPersonCharacter::RotateToTargetEnemy()
{
	if (EnemyLockedOnTo)
	{
		FVector enemyVector = EnemyLockedOnTo->GetLockOnLocation();
		enemyVector.Normalize();
		FVector vectorToEnemy = EnemyLockedOnTo->GetLockOnLocation() - GetActorLocation();
		vectorToEnemy.Normalize();

		FRotator targetRotation = vectorToEnemy.Rotation();
		FRotator newRotation = GetControlRotation();
		newRotation = UKismetMathLibrary::RLerp(newRotation, targetRotation, LockOnRotationLerp, true);
		newRotation.Roll = 0;
		Controller->SetControlRotation(newRotation);
		newRotation.Pitch = 0;
		SetActorRotation(newRotation);
	}
}

void AThirdPersonCharacter::MoveLockOnCamera()
{
	float timelinePlaybackPosition = CameraLockOnTimeline.GetPlaybackPosition() / CameraLockOnTimeline.GetTimelineLength();
	FVector targetCameraPosition = UKismetMathLibrary::VLerp(FVector::ZeroVector, LockOnTarget, timelinePlaybackPosition);
	CameraBoom->SetRelativeLocation(targetCameraPosition);
}

void AThirdPersonCharacter::OnLockOnCameraMoveFinished()
{
	
}

void AThirdPersonCharacter::AttachSocketCollisionBoxes(EAttackType attackType)
{
	// Attach collision components to sockets based on transform definitons
	// Snap rotation and transform to orient collision boxes relative to socket (In this case, the hands), but keep the world scale the same.

	FString attackSocketLocation;
	switch (attackType)
	{
		case EAttackType::MELEE_FIST:
			attackSocketLocation = TEXT("Fist_");
			break;
		case EAttackType::MELEE_KICK:
			attackSocketLocation = TEXT("Foot_");
			break;

	}

	const FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftMeleeCollisionBox->AttachToComponent(GetMesh(), attachmentRules, FName(*(attackSocketLocation + "L_Collision")));
	RightMeleeCollisionBox->AttachToComponent(GetMesh(), attachmentRules, FName(*(attackSocketLocation + "R_Collision")));
}

void AThirdPersonCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString message = __FUNCTION__ + OtherActor->GetName();
	Log(ELogLevel::INFO, message, ELogOutput::SCREEN);
	if (CharacterAudioComponent && !CharacterAudioComponent->IsPlaying())
	{
		CharacterAudioComponent->SetPitchMultiplier(.75f);
		CharacterAudioComponent->Play();
	}
}

void AThirdPersonCharacter::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString message = __FUNCTION__ + OtherActor->GetName();
	Log(ELogLevel::INFO, message, ELogOutput::SCREEN);
}

void AThirdPersonCharacter::OnAttackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FString message = __FUNCTION__ + OtherActor->GetName();
	Log(ELogLevel::INFO, message, ELogOutput::SCREEN);
}

void AThirdPersonCharacter::EndLockOn()
{
	EnemyLockedOnTo = NULL;
	CameraLockOnTimeline.Stop();
	LockOnTarget = CameraBoom->GetRelativeLocation();

	bCanUseFreeLook = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraLockOnTimeline.Reverse();
}

void AThirdPersonCharacter::AttackInput(EAttackType AttackType)
{
	/*
	FName dataTableRowName;
	FString attackSocketLocation;
	switch (AttackType) {
		case EAttackType::MELEE_FIST:
			dataTableRowName = TEXT("Punch");
			attackSocketLocation = TEXT("Fist_");
			IsAnimationBlended = true;
			IsKeyboardEnabled = true;
			break;
		case EAttackType::MELEE_KICK:
			dataTableRowName = TEXT("Kick");
			attackSocketLocation = TEXT("Foot_");
			IsAnimationBlended = false;
			break;
	}

	if (MeleeAttackDataTable)
	{
		static const FString ContextString(TEXT("Player Attack Montage Context"));
		FPlayerAttackMontage* attackMontage = MeleeAttackDataTable->FindRow<FPlayerAttackMontage>(dataTableRowName, ContextString, true);
		if (attackMontage)
		{
			int montageSectionIndex = 3;
			FString section = "Start_" + FString::FromInt(montageSectionIndex);

			const FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

			// Attach collision components to sockets based on transform definitons
			// Snap rotation and transform to orient collision boxes relative to socket (In this case, the hands), but keep the world scale the same.
			LeftMeleeCollisionBox->AttachToComponent(GetMesh(), attachmentRules, FName(*(attackSocketLocation + "L_Collision")));
			RightMeleeCollisionBox->AttachToComponent(GetMesh(), attachmentRules, FName(*(attackSocketLocation + "R_Collision")));

			CurrentAttackType = AttackType;
			UE_LOG(LogTemp, Warning, TEXT("Section: %s"), *section);

			PlayAnimMontage(attackMontage->Montage, 1.0f, FName(*section));
		}
	}
	*/

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage, 1.f, FName(TEXT("Start_1")));
	}
}

void AThirdPersonCharacter::PunchAttack()
{
	AttackInput(EAttackType::MELEE_FIST);
}

void AThirdPersonCharacter::KickAttack()
{
	AttackInput(EAttackType::MELEE_KICK);
}

void AThirdPersonCharacter::AttackStart()
{

	/*

	Log(ELogLevel::INFO, __FUNCTION__, ELogOutput::SCREEN);
	LeftMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Enabled);
	RightMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Enabled);

	// SetNotifyRigidBodyCollision = Simulation Generates Hit Events (Blueprint)
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(true);

	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(true);
	*/

	if (EquippedWeapon)
	{
		EquippedWeapon->OnWeaponAttackStart();
	}
}

void AThirdPersonCharacter::AttackEnd()
{

	/*
	Log(ELogLevel::INFO, __FUNCTION__, ELogOutput::SCREEN);
	LeftMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Disabled);
	RightMeleeCollisionBox->SetCollisionProfileName(CollisionProfile.Enabled);

	// SetNotifyRigidBodyCollision = Simulation Generates Hit Events (Blueprint)
	LeftMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	RightMeleeCollisionBox->SetNotifyRigidBodyCollision(false);
	IsKeyboardEnabled = true;

	*/

	if (EquippedWeapon)
	{
		EquippedWeapon->OnWeaponAttackEnd();
	}
}

void AThirdPersonCharacter::Log(ELogLevel LoggingLevel, FString Message, ELogOutput LogOutput)
{
	// only print when screen is selected and the GEngine object is available
	if ((LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::SCREEN) && GEngine)
	{
		// default color
		FColor LogColor = FColor::Cyan;
		// flip the color based on the type
		switch (LoggingLevel)
		{
		case ELogLevel::TRACE:
			LogColor = FColor::Green;
			break;
		case ELogLevel::DEBUG:
			LogColor = FColor::Cyan;
			break;
		case ELogLevel::INFO:
			LogColor = FColor::White;
			break;
		case ELogLevel::WARNING:
			LogColor = FColor::Yellow;
			break;
		case ELogLevel::ERROR:
			LogColor = FColor::Red;
			break;
		default:
			break;
		}
		// print the message and leave it on screen ( 2.5f controls the duration )
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, LogColor, Message);
	}

	if (LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::OUTPUT_LOG)
	{
		// flip the message type based on error level
		switch (LoggingLevel)
		{
		case ELogLevel::TRACE:
			UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::DEBUG:
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *Message);
			break;
		case ELogLevel::INFO:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		case ELogLevel::WARNING:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
			break;
		case ELogLevel::ERROR:
			UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
			break;
		}
	}
}


void AThirdPersonCharacter::CompanionFirePressed()
{
	AssignedPlayerCompanion->FireWeaponPressed();
}

void AThirdPersonCharacter::CompanionFireReleased()
{
	AssignedPlayerCompanion->FireWeaponReleased();
}
