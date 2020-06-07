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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bBelongsToPlayer;

protected:

	/** Logic when weapon hits an actor during attack. */
	void OnWeaponAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Logic when weapon overlap an actor during attack. */
	void OnWeaponAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Returns true if actor overlapped/hit during an attack is valid */
	bool IsValidAttackTarget(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void OnWeaponAttackSuccessful(AActor* ActorHit, const FHitResult& Hit);
	virtual void OnWeaponAttackSuccessful_Implementation(AActor* ActorHit, const FHitResult& Hit);

#pragma endregion

public:

	/** Method to attach weapon to character */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachToCharacter(AThirdPersonCharacter* NewWeaponOwner, const FName& AttachSocketName, bool bIsPlayerWeapon);

	/** Logic for prepping weapon for attacking */
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void OnWeaponAttackStart();
	virtual void OnWeaponAttackStart_Implementation();


	/** Logic for disabling weapon after attack ends */
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void OnWeaponAttackEnd();
	virtual void OnWeaponAttackEnd_Implementation();

};
