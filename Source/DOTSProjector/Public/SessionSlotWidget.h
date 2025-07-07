// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_RoomName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_HostName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_PlayerCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* txt_PingSpeed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_join;

	int32 SessionNumber;

	void Set(const struct FSessionInfo& InSessionInfo);

	// 세션 조인
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void JoinSession();
	
};
