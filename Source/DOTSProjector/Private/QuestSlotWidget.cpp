// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlotWidget.h"
#include "Components/RichTextBlock.h"
#include "QuestManager.h"
#include "DT_Quest.h"
#include "Components/RichTextBlockDecorator.h"

void UQuestSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QM = GetGameInstance()->GetSubsystem<UQuestManager>();
	if (QM)
	{
		QuestID = QM->QuestID_Local;
		QM->OnQuestUpdated.AddDynamic(this, &UQuestSlotWidget::OnQuestUpdated);
	}

}

void UQuestSlotWidget::SetQuestData(FQuestData& InData)
{
	Data = &InData;
	if (Data && questdescription)
	{
		questdescription->SetText(Data->QuestDescription);
	}
}

void UQuestSlotWidget::OnQuestUpdated(int32 UpdatedQuestID)
{
	if (UpdatedQuestID == QuestID)
	{
		FString CurrentText = questdescription->GetText().ToString();
		questdescription->SetText(FText::FromString("<s>" + CurrentText + "</>"));

		UE_LOG(LogTemp, Warning, TEXT("Quest %d slot marked as cleared"), QuestID);
	}
}
