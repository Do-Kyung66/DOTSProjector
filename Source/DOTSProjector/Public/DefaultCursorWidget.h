// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultCursorWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UDefaultCursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* img_defaultCursor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* img_handCursor;

	void ShowDefaultCursor();
	void ShowHandCursor();
	
};
