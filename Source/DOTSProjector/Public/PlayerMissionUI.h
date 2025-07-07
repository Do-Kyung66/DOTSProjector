// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMissionUI.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UPlayerMissionUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;

	UPROPERTY()

	TArray<class APhasmophobiaPlayer*> NumberOfPlayers;

	UPROPERTY(meta = (BindWidget))
	class  UImage* Image_CheckSanity;

	UPROPERTY(meta = (BindWidget))
	class  UImage* Image_CheckEscape;

	UPROPERTY(meta = (BindWidget))
	class  UImage* Image_CheckCamera;

	UPROPERTY()
	TArray<APlayerState*> playerArr;

	bool SanityMission = false;
	bool PictureMission = false;
};
