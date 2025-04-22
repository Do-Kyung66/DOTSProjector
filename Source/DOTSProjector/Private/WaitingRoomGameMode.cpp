// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameMode.h"
#include "WaitingRoomWidget.h"
#include "Net/UnrealNetwork.h"

AWaitingRoomGameMode::AWaitingRoomGameMode()
{
	bUseSeamlessTravel = true;
}

void AWaitingRoomGameMode::BeginPlay()
{
	/* APlayerController* PC = GetWorld()->GetFirstPlayerController();
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


void AWaitingRoomGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AWaitingRoomGameMode, WaitingRoomWidget);
}

