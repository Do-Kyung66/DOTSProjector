// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSanityUI.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UPlayerSanityUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()

	TArray<class APhasmophobiaPlayer*> NumberOfPlayers;

	UPROPERTY(meta = (BindWidget))
	class  UTextBlock* Text_SanityNum;

	UPROPERTY()
	FString name;

	UPROPERTY()
	TArray<APlayerState*> playerArr;
};
