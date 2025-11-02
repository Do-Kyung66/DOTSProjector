// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "QuestAcceptWidget.h"
#include "QuestTrackerWidget.h"

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
		// 퀘스트 슬롯 트래커에 추가하는 로직

	}
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
