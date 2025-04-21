// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SessionSlotWidget.h"
#include "Components/ScrollBox.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	// 메인
	btn_start->OnClicked.AddDynamic(this, &ULoginWidget::OnStartClicked);
	btn_exit->OnClicked.AddDynamic(this, &ULoginWidget::OnExitClicked);
	// 룸리스트
	btn_Refesh->OnClicked.AddDynamic(this, &ULoginWidget::OnRefeshClicked);
	btn_Create->OnClicked.AddDynamic(this, &ULoginWidget::OnCreateClicked);
	btn_PrivateEnter->OnClicked.AddDynamic(this, &ULoginWidget::OnPrivateEnterClicked);
	gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotWidget);
	// room join
	
	// 방만들기
	btn_createRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateRoom);
	CheckBox_PublicRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPublicheckBoxChanged);
	CheckBox_PrivateRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPrivateCheckBoxChanged);
	CheckBox_PublicRoom->SetCheckedState(ECheckBoxState::Checked);
	CheckBox_PrivateRoom->SetCheckedState(ECheckBoxState::Unchecked);
	bIsPublic = true;
	// 프라이빗 룸 키
	// room join

	// 웨이팅 룸
	// 게임 시작
	btn_leave->OnClicked.AddDynamic(this, &ULoginWidget::OnStartClicked);

	// 뒤로가기 버튼
	btn_BackMain->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);
	btn_BackRoomList_1->OnClicked.AddDynamic(this, &ULoginWidget::BackToRoomList);
	btn_BackRoomList_2->OnClicked.AddDynamic(this, &ULoginWidget::BackToRoomList);
}

void ULoginWidget::OnStartClicked()
{
	// 방 리스트로 이동
	WidgetSwitcher->SetActiveWidgetIndex(1);
	OnRefeshClicked();
}

void ULoginWidget::OnExitClicked()
{
	// 게임 종료
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
}

void ULoginWidget::OnRefeshClicked()
{
	// 방 리스트 업데이트
	Scroll_RoomList->ClearChildren();
	if (gi != nullptr)
	{
		gi->FindOtherSession();
	}
}

void ULoginWidget::OnCreateClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void ULoginWidget::OnPrivateEnterClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
}

// 방리스트에 슬롯 추가
void ULoginWidget::AddSlotWidget(const struct FSessionInfo& InSessionInfo)
{
	auto slot = CreateWidget<USessionSlotWidget>(this, SessionInfoWidget);
	slot->Set(InSessionInfo);

	Scroll_RoomList->AddChild(slot);
}

void ULoginWidget::CreateRoom()
{
	if(!gi || edit_roomName->GetText().IsEmpty()) return;
	if(CheckBox_PublicRoom->GetCheckedState() == ECheckBoxState::Unchecked && CheckBox_PrivateRoom->GetCheckedState() == ECheckBoxState::Unchecked) return; // 두개 선택은 막아둠
	
	gi->CreateMySession(edit_roomName->GetText().ToString(), bIsPublic);
	WidgetSwitcher->SetActiveWidgetIndex(4); // 웨이팅룸 입장
}

void ULoginWidget::OnPublicheckBoxChanged(bool Check)
{
	// 만약에 privateroom에 체크가 되어 있는데 publicroom을 체크하게 되면 privateroom은 체크가 해제되어야 한다.
	if (CheckBox_PublicRoom->GetCheckedState() == ECheckBoxState::Checked)
	{
		bIsPublic = true;
		CheckBox_PrivateRoom->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void ULoginWidget::OnPrivateCheckBoxChanged(bool Check)
{
	if (CheckBox_PrivateRoom->GetCheckedState() == ECheckBoxState::Checked)
	{
		bIsPublic = false;
		CheckBox_PublicRoom->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void ULoginWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULoginWidget::BackToRoomList()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	OnRefeshClicked();
}
