// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Net/UnrealNetwork.h"
#include "WaitingRoomWidget.h"
#include "PhasmophobiaGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "LoginWidget.h"


APhasmophobiaPlayerController::APhasmophobiaPlayerController()
{

	//bShowMouseCursor = true;
	bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

	//DefaultMouseCursor = EMouseCursor::Default;

}


void APhasmophobiaPlayerController::BeginPlay()
{
	if (HasAuthority())
	{
		gm = Cast<APhasmophobiaGameMode>(GetWorld()->GetAuthGameMode());
	}
	
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

	if (LoginWidgetClass)
	{
		ULoginWidget* LoginWidget = CreateWidget<ULoginWidget>(this, LoginWidgetClass);
		if (LoginWidget)
		{
			LoginWidget->AddToViewport();
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
		if (PC && !PC->IsLocalController()) // Ŭ���̾�Ʈ���׸�
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

void APhasmophobiaPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	// �����ڰ� �÷��̾��� ��ġ�� ������ �� �ֵ��� �÷��̾� ������ �����´�.
	auto player = GetPawn();

	if (player)
	{
		UnPossess();
		// ������ ����
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform(), params);

		// ����(possess)
		Possess(spectator);

		// ���� �÷��̾� ����
		player->Destroy();

		//FTimerHandle handle;

		//GetWorldTimerManager().SetTimer(handle, this, &APhasmophobiaPlayerController::ServerRPC_RespawnPlayer_Implementation, 5.0, false);

	}
}

void APhasmophobiaPlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	auto player = GetPawn();
	UnPossess();
	player->Destroy();

	gm->RestartPlayer(this);
}
