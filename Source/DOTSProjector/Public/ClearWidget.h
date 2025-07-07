// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	class UButton* ClearButton;

	UFUNCTION()
	void OnClearButtonClicked();

	virtual void NativeConstruct() override;
};
