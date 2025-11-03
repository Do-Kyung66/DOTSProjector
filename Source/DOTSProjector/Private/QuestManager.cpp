// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "QuestAcceptWidget.h"
#include "QuestTrackerWidget.h"
#include "QuestSlotWidget.h"

void UQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!QuestDataTable)
	{
		QuestDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Quest/Data/DT_QuestData.DT_QuestData"));
		if (!QuestDataTable)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load QuestDataTable at runtime!"));
		}
	}
}

void UQuestManager::Deinitialize()
{
	Super::Deinitialize();
}

void UQuestManager::CreateQuestAcceptWidget()
{
	if (!QuestAcceptWidgetClass)
	{
		QuestAcceptWidgetClass = LoadClass<UQuestAcceptWidget>(nullptr, TEXT("/Game/Quest/Widgets/WBP_QuestAcceptWindow.WBP_QuestAcceptWindow_C"));
	}

	QuestAcceptWidget = CreateWidget<UQuestAcceptWidget>(GetWorld(), QuestAcceptWidgetClass);
	if (QuestAcceptWidget)
	{
		QuestAcceptWidget->AddToViewport();
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(QuestAcceptWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		PC->SetPause(true);
	}

	const FQuestData* NextQuest = GetQuestDataByID(QuestID_Local);
	if (NextQuest && NextQuest->bCanUseLineTrace)
	{
		ActivateTraceQuest(QuestID_Local);
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Activated for Quest ID: %d"), QuestID_Local);
	}
}

void UQuestManager::DestroyQuestAcceptWidget()
{
	if (QuestAcceptWidget)
	{
		QuestAcceptWidget->RemoveFromParent();
		QuestAcceptWidget = nullptr;
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;

		PC->SetPause(false);
	}
}

void UQuestManager::CreateQuestTrackerWidget()
{
	if (!QuestTrackerWidgetClass)
	{
		QuestTrackerWidgetClass = LoadClass<UQuestTrackerWidget>(nullptr, TEXT("/Game/Quest/Widgets/WBP_QuestTracker.WBP_QuestTracker_C"));
	}

	QuestTrackerWidget = CreateWidget<UQuestTrackerWidget>(GetWorld(), QuestTrackerWidgetClass);
	if (QuestTrackerWidget)
	{
		QuestTrackerWidget->AddToViewport();
	}
}

void UQuestManager::AcceptQuest(int32 QuestID)
{
	FQuestData* Data = GetQuestDataByID(QuestID);
	if (Data)
	{
		UE_LOG(LogTemp, Warning, TEXT("Accepted Quest ID: %d"), QuestID);

		Data->QuestAccepted = true;
		ActiveQuests.Add(*Data);


		// 퀘스트 슬롯 생성
		if (!QuestSlotWidgetClass)
		{
			QuestSlotWidgetClass = LoadClass<UQuestSlotWidget>(
				nullptr,
				TEXT("/Game/Quest/Widgets/WBP_QuestSlot.WBP_QuestSlot_C")
			);
		}

		if (QuestTrackerWidget && QuestSlotWidgetClass)
		{
			UQuestSlotWidget* QuestSlot = CreateWidget<UQuestSlotWidget>(GetWorld(), QuestSlotWidgetClass);
			if (QuestSlot)
			{
				QuestSlot->SetQuestData(*Data);

				QuestTrackerWidget->AddQuestSlot(QuestSlot);
			}
		}
	}
}

void UQuestManager::CompleteQuest(int32 CurrentQuestID)
{
	for (FQuestData& Quest : ActiveQuests)
	{
		if (Quest.QuestID == CurrentQuestID)
		{
			Quest.QuestCompleted = true;
			UE_LOG(LogTemp, Warning, TEXT("Quest %d completed!"), CurrentQuestID);

			OnQuestUpdated.Broadcast(CurrentQuestID); // UI에 알림

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("HandleQuestCompleteDelay"), CurrentQuestID);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 3.0f, false);

			break;
		}
	}
}

void UQuestManager::HandleQuestCompleteDelay(int32 CompletedQuestID)
{
	QuestID_Local = CompletedQuestID + 1;
	if (QuestID_Local == 4)
	{
		QuestID_Local = -1;
		return;
	}
	CreateQuestAcceptWidget();
}

void UQuestManager::ActivateTraceQuest(int32 QuestID)
{
	ActiveTraceQuestID = QuestID;
	bCanUseLineTrace = true;
}

void UQuestManager::DeactivateTraceQuest()
{
	ActiveTraceQuestID = -1;
	bCanUseLineTrace = false;
}

FQuestData* UQuestManager::GetQuestDataByID(int32 QuestID)
{
	if (!QuestDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestDataTable is NULL!"));
		return nullptr;
	}

	// RowName을 ID 숫자 기반으로 생성
	FName RowName = *FString::Printf(TEXT("%d"), QuestID);

	FQuestData* Data = QuestDataTable->FindRow<FQuestData>(RowName, TEXT("QM"));

	return Data;
}
