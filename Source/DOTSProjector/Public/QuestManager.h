// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DT_Quest.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DOTSPROJECTOR_API UQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestWidget")
	TSubclassOf<class UQuestAcceptWidget> QuestAcceptWidgetClass;
	UPROPERTY()
	TObjectPtr<class UQuestAcceptWidget> QuestAcceptWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestWidget")
	TSubclassOf<class UQuestTrackerWidget> QuestTrackerWidgetClass;
	UPROPERTY()
	TObjectPtr<class UQuestTrackerWidget> QuestTrackerWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestWidget")
	TSubclassOf<class UQuestSlotWidget> QuestSlotWidgetClass;

	



public:
	void CreateQuestAcceptWidget();
	void DestroyQuestAcceptWidget();
	void CreateQuestTrackerWidget();
	void AcceptQuest(int32 QuestID);

	FQuestData* GetQuestDataByID(int32 QuestID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestID_Local = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	TObjectPtr<UDataTable> QuestDataTable;
	
};
