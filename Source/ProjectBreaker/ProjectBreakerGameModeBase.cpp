// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBreakerGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AProjectBreakerGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}