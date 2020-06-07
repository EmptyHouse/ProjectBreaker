// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class PROJECTBREAKER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AWeaponBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	/** Static mesh component of this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	/** Damage to apply if weapon collides with an enemy */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseWeaponDamage;

#pragma endregion

public:

	/** Method to attach weapon to character */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachToCharacter(AThirdPersonCharacter* NewWeaponOwner, const FName& AttachSocketName);


};
