// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlotWidget.h"
#include "Components/TextBlock.h"
#include "QuestManager.h"
#include "DT_Quest.h"

void UQuestSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UQuestSlotWidget::SetQuestData(FQuestData& InData)
{
	Data = &InData;
	if (Data && questdescription)
	{
		questdescription->SetText(Data->QuestDescription);
	}
}
