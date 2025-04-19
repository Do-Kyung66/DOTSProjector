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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_createRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_roomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UCheckBox* CheckBox_PublicRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UCheckBox* CheckBox_PrivateRoom;

	UPROPERTY()
	class UNetGameInstance* gi;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateRoom();

	// 비공개방 체크되면 호출되는 함수
	bool bIsPublic;

	UFUNCTION()
	void OnPublicheckBoxChanged(bool Check);
	UFUNCTION()
	void OnPrivateCheckBoxChanged(bool Check);
};
