// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrackerWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "QuestSlotWidget.h"

void UQuestTrackerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	questlist_title->SetVisibility(ESlateVisibility::Hidden);
}

void UQuestTrackerWidget::AddQuestSlot(UQuestSlotWidget* QuestSlot)
{
	if(!title_visible)
	{ 
		questlist_title->SetVisibility(ESlateVisibility::Visible);
	}

	if (quest_listbox && QuestSlot)
	{
		quest_listbox->AddChildToVerticalBox(Cast<UWidget>(QuestSlot));
	}
}
