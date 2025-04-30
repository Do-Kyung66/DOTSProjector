// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomWidget.h"
#include "Components/Button.h"
#include "NetGameInstance.h"
#include "Components/TextBlock.h"
#include "DOTSProjector.h"
#include "PhasmophobiaPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "LoginWidget.h"
#include "NetGameStateBase.h"



void UWaitingRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	
	btn_leave->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnLeaveClicked);
	btn_gamestart->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnGameStartClicked);

	//txt_CodeLabel->SetVisibility(ESlateVisibility::Hidden);
	//txt_privateKey->SetVisibility(ESlateVisibility::Hidden);
	FSessionInfo& sessionInfo = gi->sessionInfo;
	txt_RoomName->SetText(FText::FromString(sessionInfo.roomName));
	
	PRINTLOG(TEXT("roomname : %s"), *txt_RoomName->GetText().ToString());
	//PRINTLOG(TEXT("bIsPrivate : %d"), sessionInfo.bIsPrivate);
	//PRINTLOG(TEXT("roomcode : %s"), *sessionInfo.roomCode);

	/*if (sessionInfo.bIsPrivate == true)
	{
		txt_CodeLabel->SetVisibility(ESlateVisibility::Visible);
		txt_privateKey->SetVisibility(ESlateVisibility::Visible);
		txt_privateKey->SetText(FText::FromString(sessionInfo.roomCode));
		PRINTLOG(TEXT("code : %s"), *txt_privateKey->GetText().ToString());
	}*/
	// txt_Users->SetText(FText::FromString(TEXT("HELLO")));
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWaitingRoomWidget::UpdatePlayerList);
}	

void UWaitingRoomWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdatePlayerList();

	ANetGameStateBase* GS = GetWorld()->GetGameState<ANetGameStateBase>();
	if (GS)
	{
		txt_RoomName->SetText(FText::FromString(GS->RoomName));
	}
	
}

void UWaitingRoomWidget::OnLeaveClicked()
{

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	 if (PC)
	 {
		this->RemoveFromParent();
		if (LoginWidget)
		{
			Widget = CreateWidget<ULoginWidget>(PC, LoginWidget);
			Widget->AddToViewport();
			Widget->OnStartClicked();
		}
	}

	PRINTLOG(TEXT("leavebutton clicked"));

}

void UWaitingRoomWidget::OnGameStartClicked()
{
	PRINTLOG(TEXT("OnGameStartClicked!!!"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PRINTLOG(TEXT("PlayerController is"));

		APhasmophobiaPlayerController* myPC = Cast<APhasmophobiaPlayerController>(PC);
		if (myPC)
		{
			PRINTLOG(TEXT("APhasmophobiaPlayerController is"));

			myPC->ServerRPC_RequestStartGame();
			PRINTLOG(TEXT("move battle map!!!!"));
		}
	}
}

void UWaitingRoomWidget::UpdatePlayerList()
{
	// 플레이어 리스트 출력
	TArray<APlayerState*> playerArr = GetWorld()->GetGameState()->PlayerArray;
	UE_LOG(LogTemp, Warning, TEXT("PlayerArray Num: %d"), GetWorld()->GetGameState()->PlayerArray.Num());
	FString name;
	for (APlayerState* pState : playerArr)
	{
		name.Append(FString::Printf(TEXT("%s\n"), *pState->GetPlayerName()));
	}

	txt_Users->SetText(FText::FromString(name));
}

void UWaitingRoomWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UWaitingRoomWidget, txt_RoomName);
	// DOREPLIFETIME(UWaitingRoomWidget, btn_leave);

}
