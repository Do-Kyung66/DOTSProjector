// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_Quest.h"
#include "QuestTrackerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UQuestTrackerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* questlist_title;
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* quest_listbox;

	bool title_visible = false;

	class UQuestManager* QM = nullptr;
	FQuestData* Data;

	void AddQuestSlot(class UQuestSlotWidget* QuestSlot);
	
};
