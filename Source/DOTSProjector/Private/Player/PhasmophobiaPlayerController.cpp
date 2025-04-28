// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Net/UnrealNetwork.h"
#include "WaitingRoomWidget.h"


APhasmophobiaPlayerController::APhasmophobiaPlayerController()
{

	//bShowMouseCursor = true;
	bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

	//DefaultMouseCursor = EMouseCursor::Default;

}

void APhasmophobiaPlayerController::BeginPlay()
{
	if (IsLocalController() && WaitingRoomWidgetClass)
	{
		UWaitingRoomWidget* UI = CreateWidget<UWaitingRoomWidget>(this, WaitingRoomWidgetClass);
		if (UI)
		{
			UI->bIsFocusable = true;
			UI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("waiting room!!!!!"));

		}
	}

	/*APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		if (WaitingRoomWidget)
		{
			Widget = CreateWidget<UWaitingRoomWidget>(PC, WaitingRoomWidget);
			Widget->bIsFocusable = true;
			Widget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(Widget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}*/

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
	DOREPLIFETIME(APhasmophobiaPlayerController, bCanInteract);
}

void APhasmophobiaPlayerController::ServerRPC_RequestStartGame_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && !PC->IsLocalController()) // 클라이언트한테만
		{
			PC->ClientTravel("/Game/FirstPerson/Maps/FirstPersonMap", TRAVEL_Absolute);
		}
	}
	
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(TEXT("/Game/FirstPerson/Maps/FirstPersonMap?listen"));
	}
}
