// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomWidget.h"
#include "Components/Button.h"
#include "NetGameInstance.h"
#include "Components/TextBlock.h"
#include "DOTSProjector.h"
#include "PhasmophobiaPlayerController.h"
#include "Net/UnrealNetwork.h"

void UWaitingRoomWidget::NativeConstruct()
{
	gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	
	btn_leave->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnLeaveClicked);
	btn_gamestart->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnGameStartClicked);

	if (!btn_gamestart) 
	{
		PRINTLOG(TEXT("btngamestart nullptr"));

	}
	if (btn_gamestart)
	{
		PRINTLOG(TEXT("btngamestart"));

	}
	txt_CodeLabel->SetVisibility(ESlateVisibility::Hidden);
	txt_privateKey->SetVisibility(ESlateVisibility::Hidden);
	FSessionInfo& sessionInfo = gi->sessionInfo;
	txt_RoomName->SetText(FText::FromString(sessionInfo.roomName));
	PRINTLOG(TEXT("roomname : %s"), *txt_RoomName->GetText().ToString());
	PRINTLOG(TEXT("bIsPrivate : %d"), sessionInfo.bIsPrivate);
	PRINTLOG(TEXT("roomcode : %s"), *sessionInfo.roomCode);

	if (sessionInfo.bIsPrivate == true)
	{	
		txt_CodeLabel->SetVisibility(ESlateVisibility::Visible);
		txt_privateKey->SetVisibility(ESlateVisibility::Visible);
		txt_privateKey->SetText(FText::FromString(sessionInfo.roomCode));
		PRINTLOG(TEXT("code : %s"), *txt_privateKey->GetText().ToString());
	}
	

}	

void UWaitingRoomWidget::OnLeaveClicked()
{
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

void UWaitingRoomWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(UWaitingRoomWidget, txt_RoomName);
	// DOREPLIFETIME(UWaitingRoomWidget, btn_leave);

}
