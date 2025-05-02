// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameMode.h"
#include "WaitingRoomWidget.h"
#include "Net/UnrealNetwork.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"

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


void AWaitingRoomGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Warning, TEXT("A player has left the room"));

	CheckRoomEmpty();
}

void AWaitingRoomGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AWaitingRoomGameMode::CheckRoomEmpty()
{
	int32 NumPlayers = GetNumPlayers();
	UE_LOG(LogTemp, Warning, TEXT("Current Players: %d"), NumPlayers);

	if (NumPlayers <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Room is empty. Destroying session..."));
		DestroySession();
	}
}

void AWaitingRoomGameMode::DestroySession()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			Session->DestroySession(NAME_GameSession);
		}
	}
}

