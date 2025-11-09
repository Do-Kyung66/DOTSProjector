// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DT_Quest.h"
#include "QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, int32, QuestID);

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
	void DestroyQuestTrackerWidget();
	void AcceptQuest(int32 QuestID);
	void CompleteQuest(int32 CurrentQuestID);
	UFUNCTION()
	void HandleQuestCompleteDelay(int32 CompletedQuestID);
	void ActivateTraceQuest(int32 QuestID);
	void DeactivateTraceQuest();

	FQuestData* GetQuestDataByID(int32 QuestID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestID_Local = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestData")
	TObjectPtr<UDataTable> QuestDataTable;

	UPROPERTY()
	TArray<FQuestData> ActiveQuests;
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest|Trace")
	bool bCanUseLineTrace = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest|Trace")
	int32 ActiveTraceQuestID = -1;
	bool CaptureGhostPhoto = false;

	// 퀘스트2 사운드
	UPROPERTY(EditAnywhere, Category = "Quest|Sound")
	TArray<USoundBase*> MirrorNoiseSounds;

	FTimerHandle MirrorNoiseTimerHandle;
	int32 CurrentNoiseIndex = 0;

	void StartMirrorNoise();
	void PlayMirrorNoise();
	void StopMirrorNoise();
	
	bool bIsPlayingMirrorNoise = false;

};
