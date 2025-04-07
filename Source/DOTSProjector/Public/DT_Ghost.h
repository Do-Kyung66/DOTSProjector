// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IGhostBehaviorStrategy.h"
#include "DT_Ghost.generated.h"


USTRUCT(BlueprintType)
struct FGhostBehaviorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Behavior")
	TSubclassOf<UGhostBehaviorStrategy> BehaviorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Behavior")
	float Probability;
};

USTRUCT(BlueprintType)
struct FGhostData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	FName GhostName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	UStaticMesh* GhostMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	TArray<FGhostBehaviorData> BehaviorDatas;
};