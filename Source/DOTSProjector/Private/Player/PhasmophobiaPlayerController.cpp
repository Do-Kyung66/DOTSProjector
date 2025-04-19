// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Net/UnrealNetwork.h"


APhasmophobiaPlayerController::APhasmophobiaPlayerController()
{

	//bShowMouseCursor = true;
	bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

	//DefaultMouseCursor = EMouseCursor::Default;

}

void APhasmophobiaPlayerController::BeginPlay()
{
	// 커서 표시 및 입력 모드 설정



}

void APhasmophobiaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void APhasmophobiaPlayerController::SetCursorForInteraction(bool bIsInteractable, AActor* tempItem)
{
	if (bIsInteractable)
	{
		bCanInteract = true;
		TargetItem = tempItem;
		//CurrentMouseCursor = EMouseCursor::Hand;
		UE_LOG(LogTemp, Warning, TEXT("Cursor : Hand"));
	}
	else
	{
		bCanInteract = false;
		TargetItem = nullptr;
		//CurrentMouseCursor = EMouseCursor::Default;
		UE_LOG(LogTemp, Warning, TEXT("Cursor : Default"));
	}


	/*APhasmophobiaHUD* HUD = Cast<APhasmophobiaHUD>(GetHUD());
	if (HUD && HUD->bIsJournalOpen) return;*/

	//if (bIsInteractable)
	//{
	//	bCanInteract = true;
	//	TargetItem = tempItem;
	//	CurrentMouseCursor = EMouseCursor::Hand;
	//	UE_LOG(LogTemp, Warning, TEXT("Cursor : Hand"));
	//}
	//else
	//{
	//	bCanInteract = false;
	//	TargetItem = nullptr;
	//	CurrentMouseCursor = EMouseCursor::Default;
	//	UE_LOG(LogTemp, Warning, TEXT("Cursor : Default"));
	//}

	//FInputModeGameAndUI InputMode;
	
}


void APhasmophobiaPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APhasmophobiaPlayerController, TargetItem);
}

