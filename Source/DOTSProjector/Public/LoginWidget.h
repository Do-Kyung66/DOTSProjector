// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	// ����
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_start;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_exit;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_userName;

	// �� ����Ʈ
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Find;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Create;

	/*UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_PrivateEnter;*/

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* Scroll_RoomList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionSlotWidget> SessionInfoWidget;

	// �� �����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_createRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_roomName;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UCheckBox* CheckBox_PublicRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UCheckBox* CheckBox_PrivateRoom;*/

	// private key �Է�
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_privatekey;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_join;*/

	
	// �ڷΰ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_BackMain;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_BackRoomList_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_BackRoomList_2;
	
	UPROPERTY()
	class UNetGameInstance* gi;

public:
	virtual void NativeConstruct() override;

	// ����
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnExitClicked();

	// �� ����Ʈ
	UFUNCTION()
	void OnFindClicked();

	UFUNCTION()
	void OnCreateClicked();

	UFUNCTION()
	void OnPrivateEnterClicked();

	UFUNCTION()
	void AddSlotWidget(const struct FSessionInfo& InSessionInfo);

	// �� �����
	UFUNCTION()
	void CreateRoom();

	// ������� üũ�Ǹ� ȣ��Ǵ� �Լ�
	bool bIsPrivate;
	int32 SessionNumber;

	//UFUNCTION()
	//void OnPublicheckBoxChanged(bool Check);
	//UFUNCTION()
	//void OnPrivateCheckBoxChanged(bool Check);

	// private room enter Ŭ��
	UFUNCTION()
	void JoinSession();

	// �ڷΰ��� �Լ�
	UFUNCTION()
	void BackToMain();

	UFUNCTION()
	void BackToRoomList();
};
