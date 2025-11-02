// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_Quest.h"
#include "QuestSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UQuestSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* questdescription;

	class UQuestManager* QM = nullptr;
	FQuestData* Data;

	void SetQuestData(FQuestData& InData);
	
};
