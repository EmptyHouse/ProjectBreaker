// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackStartNotifyState.h"
#include "ThirdPersonCharacter.h"
#include "Engine.h"

void UAttackStartNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner())
	{
		AThirdPersonCharacter* playerCharacter = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());
		if (playerCharacter != NULL)
		{
			playerCharacter->AttackStart();
			IsKeyboardEnabled = playerCharacter->GetCurrentAttackType() == EAttackType::MELEE_FIST;
		}
	}
}

void UAttackStartNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, __FUNCTION__);

	if (MeshComp && MeshComp->GetOwner())
	{
		AThirdPersonCharacter* playerCharacter = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());
		if (playerCharacter != NULL)
		{
			playerCharacter->SetIsKeyboardEnabled(true);
			playerCharacter->AttackEnd();
		}
	}
}

void UAttackStartNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (MeshComp && MeshComp->GetOwner())
	{
		AThirdPersonCharacter* playerCharacter = Cast<AThirdPersonCharacter>(MeshComp->GetOwner());
		if (playerCharacter)
		{
			playerCharacter->SetIsKeyboardEnabled(IsKeyboardEnabled);
		}
	}
}
