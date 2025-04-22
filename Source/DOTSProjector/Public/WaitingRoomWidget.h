// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UWaitingRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_CodeLabel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_privateKey;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_leave;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_ready;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_gamestart;

	UPROPERTY()
	class UNetGameInstance* gi;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnLeaveClicked();
	UFUNCTION()
	void OnGameStartClicked();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
