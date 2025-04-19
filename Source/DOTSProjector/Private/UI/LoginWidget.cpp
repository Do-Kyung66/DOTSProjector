// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	btn_createRoom->OnClicked.AddDynamic(this, &ULoginWidget::CreateRoom);
	CheckBox_PublicRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPublicheckBoxChanged);
	CheckBox_PrivateRoom->OnCheckStateChanged.AddDynamic(this, &ULoginWidget::OnPrivateCheckBoxChanged);

	CheckBox_PublicRoom->SetCheckedState(ECheckBoxState::Checked);
	CheckBox_PrivateRoom->SetCheckedState(ECheckBoxState::Unchecked);
	bIsPublic = true;
}

void ULoginWidget::CreateRoom()
{
	if(!gi || edit_roomName->GetText().IsEmpty()) return;
	if(CheckBox_PublicRoom->GetCheckedState() == ECheckBoxState::Unchecked && CheckBox_PrivateRoom->GetCheckedState() == ECheckBoxState::Checked) return;
	
	gi->CreateMySession(edit_roomName->GetText().ToString(), bIsPublic);
}

void ULoginWidget::OnPublicheckBoxChanged(bool Check)
{
	// ���࿡ privateroom�� üũ�� �Ǿ� �ִµ� publicroom�� üũ�ϰ� �Ǹ� privateroom�� üũ�� �����Ǿ�� �Ѵ�.
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
