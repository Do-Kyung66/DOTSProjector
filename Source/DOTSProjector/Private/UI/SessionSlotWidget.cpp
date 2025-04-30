// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlotWidget.h"
#include "NetGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DOTSProjector.h"

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_join->OnClicked.AddDynamic(this, &USessionSlotWidget::JoinSession);
}

void USessionSlotWidget::Set(const struct FSessionInfo& InSessionInfo)
{
	txt_RoomName->SetText(FText::FromString(InSessionInfo.roomName));
	txt_HostName->SetText(FText::FromString(InSessionInfo.hostName));

	txt_PlayerCount->SetText(FText::FromString(InSessionInfo.playerCount));
	txt_PingSpeed->SetText(FText::FromString(FString::Printf(TEXT("%dms"), InSessionInfo.pingSpeed)));

	SessionNumber = InSessionInfo.index;
}

void USessionSlotWidget::JoinSession()
{
	PRINTLOG(TEXT("111111 join!!!!!!!!!!!!!"));
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->JoinSelectedSession(SessionNumber);
		PRINTLOG(TEXT("22222 join!!!!!!!!!!!!!"));
	}
}
