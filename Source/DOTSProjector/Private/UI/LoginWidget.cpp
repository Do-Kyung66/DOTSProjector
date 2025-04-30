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

	if(!gi || edit_roomName->GetText().IsEmpty()) return;
	gi->CreateMySession(edit_roomName->GetText().ToString());
	// ����
	btn_start->OnClicked.AddDynamic(this, &ULoginWidget::OnStartClicked);
	btn_exit->OnClicked.AddDynamic(this, &ULoginWidget::OnExitClicked);
	// �븮��Ʈ
	btn_Find->OnClicked.AddDynamic(this, &ULoginWidget::OnFindClicked);
	btn_Create->OnClicked.AddDynamic(this, &ULoginWidget::OnCreateClicked);
	//btn_PrivateEnter->OnClicked.AddDynamic(this, &ULoginWidget::OnPrivateEnterClicked);
	gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddSlotWidget);
	// room join
	
	// �游���
	btn_createRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateRoom);
	/*CheckBox_PublicRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPublicheckBoxChanged);
	CheckBox_PrivateRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPrivateCheckBoxChanged);
	CheckBox_PublicRoom->SetCheckedState(ECheckBoxState::Checked);
	CheckBox_PrivateRoom->SetCheckedState(ECheckBoxState::Unchecked);
	bIsPrivate = false;*/
	// �����̺� �� Ű
	// room join
	//btn_join->OnClicked.AddDynamic(this, &ULoginWidget::JoinSession);

	// �ڷΰ��� ��ư
	btn_BackMain->OnClicked.AddDynamic(this, &ULoginWidget::BackToMain);
	btn_BackRoomList_1->OnClicked.AddDynamic(this, &ULoginWidget::BackToRoomList);
	btn_BackRoomList_2->OnClicked.AddDynamic(this, &ULoginWidget::BackToRoomList);


}

void ULoginWidget::OnStartClicked()
{
	// �� ����Ʈ�� �̵�
	WidgetSwitcher->SetActiveWidgetIndex(1);
	OnFindClicked();

	if (edit_userName->GetText().IsEmpty() == false)
	{
		gi->mySessionName = edit_userName->GetText().ToString();
	}


}

void ULoginWidget::OnExitClicked()
{
	// ���� ����
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
}

void ULoginWidget::OnFindClicked()
{
	// �� ����Ʈ ������Ʈ
	Scroll_RoomList->ClearChildren();
	if (gi != nullptr)
	{
		gi->FindOtherSession();
	}
}

void ULoginWidget::OnCreateClicked()
{
	if (edit_userName->GetText().IsEmpty() == false)
	{
		gi->mySessionName = edit_userName->GetText().ToString();
	}
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void ULoginWidget::OnPrivateEnterClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
}

// �渮��Ʈ�� ���� �߰�
void ULoginWidget::AddSlotWidget(const struct FSessionInfo& InSessionInfo)
{
	for (UWidget* child : Scroll_RoomList->GetAllChildren())
	{
		if (USessionSlotWidget* slotWidget = Cast<USessionSlotWidget>(child))
		{
			if (slotWidget->SessionNumber == InSessionInfo.index)
			{
				// �̹� �ִ� ������ ����
				return;
			}
		}
	}
	auto slot = CreateWidget<USessionSlotWidget>(this, SessionInfoWidget);
	slot->Set(InSessionInfo);
	Scroll_RoomList->AddChild(slot);
}

void ULoginWidget::CreateRoom()
{
	if(!gi || edit_roomName->GetText().IsEmpty()) return;
	//if(CheckBox_PublicRoom->GetCheckedState() == ECheckBoxState::Unchecked && CheckBox_PrivateRoom->GetCheckedState() == ECheckBoxState::Unchecked) return; // �ΰ� ������ ���Ƶ�
	
	gi->CreateMySession(edit_roomName->GetText().ToString());
	// WidgetSwitcher->SetActiveWidgetIndex(4); // �����÷� ����
}

//void ULoginWidget::OnPublicheckBoxChanged(bool Check)
//{
//	// ���࿡ privateroom�� üũ�� �Ǿ� �ִµ� publicroom�� üũ�ϰ� �Ǹ� privateroom�� üũ�� �����Ǿ�� �Ѵ�.
//	/*if (CheckBox_PublicRoom->GetCheckedState() == ECheckBoxState::Checked)
//	{
//		bIsPrivate = false;
//		CheckBox_PrivateRoom->SetCheckedState(ECheckBoxState::Unchecked);
//	}*/
//}
//void ULoginWidget::OnPrivateCheckBoxChanged(bool Check)
//{
//	/*if (CheckBox_PrivateRoom->GetCheckedState() == ECheckBoxState::Checked)
//	{
//		bIsPrivate = true;
//		CheckBox_PublicRoom->SetCheckedState(ECheckBoxState::Unchecked);
//	}*/
//}

void ULoginWidget::JoinSession()
{
	// �̹�ư�� �����̺� �� ���� ��ư
	// Ű���� �´� ���� �ε����� ã�Ƽ� �����ϸ� �ȴ�. 
	// ���࿡ Ű���� ���� ������ ���� �Ұ�
	//FString Code = edit_privatekey->GetText().ToString();
	/*if (gi)
	{
		gi->JoinPrivateRoom(Code);
	}*/

}

void ULoginWidget::BackToMain()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULoginWidget::BackToRoomList()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	OnFindClicked();
}
