// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DT_Quest.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
    GENERATED_BODY()

public:
    FQuestData()
        : QuestID(-1)
        , QuestDescription(FText::FromString(TEXT("")))
        , QuestAccepted(false)
		, bCanUseLineTrace(false)
		, QuestCompleted(false)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
    int32 QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
    FText QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
    bool QuestAccepted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
    bool bCanUseLineTrace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
    bool QuestCompleted;
};