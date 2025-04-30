// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaGameMode.h"
#include "PhasmophobiaPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

APhasmophobiaGameMode::APhasmophobiaGameMode()
{
	PlayerControllerClass = APhasmophobiaPlayerController::StaticClass();
}

void APhasmophobiaGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    static int32 PlayerIndex = 1;

    if (NewPlayer && NewPlayer->PlayerState)
    {
        FString Name = FString::Printf(TEXT("Player%d"), PlayerIndex++);
        NewPlayer->PlayerState->SetPlayerName(Name);
    }
}
