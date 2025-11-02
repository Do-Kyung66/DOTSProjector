#include "QuestAcceptWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "QuestManager.h"
#include "DT_Quest.h"

void UQuestAcceptWidget::NativeConstruct()
{
	if (button_accept)
		button_accept->OnClicked.AddDynamic(this, &UQuestAcceptWidget::OnButtonAcceptClicked);

	QM = GetGameInstance()->GetSubsystem<UQuestManager>();
	if (QM)
	{
		QuestID = QM->QuestID_Local;
		Data = QM->GetQuestDataByID(QuestID);

		UE_LOG(LogTemp, Warning, TEXT("QuestID: %d"), QuestID);
		UE_LOG(LogTemp, Warning, TEXT("Data pointer: %p"), Data);

		if (Data && questdescription)
		{
			questdescription->SetText(Data->QuestDescription);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Data or questdescription is NULL!"));
		}
	}
}

void UQuestAcceptWidget::OnButtonAcceptClicked()
{
	if (QM)
	{
		QM->AcceptQuest(QuestID);
		QM->DestroyQuestAcceptWidget();
	}
}
