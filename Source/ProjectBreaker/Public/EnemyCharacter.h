// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponBase.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROJECTBREAKER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowProtectedAcccess = "true"))
	UAnimMontage* AttackMontage;

	/** Class of default weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> DefaultWeaponClass;

	/** Currently equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowProtectedAccess = true), Category = "Weapon")
	AWeaponBase* EquippedWeapon;

public:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(AWeaponBase* Weapon);

};
