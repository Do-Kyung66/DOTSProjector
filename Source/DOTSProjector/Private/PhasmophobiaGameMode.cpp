// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaGameMode.h"
#include "PhasmophobiaPlayerController.h"

APhasmophobiaGameMode::APhasmophobiaGameMode()
{
	PlayerControllerClass = APhasmophobiaPlayerController::StaticClass();
}