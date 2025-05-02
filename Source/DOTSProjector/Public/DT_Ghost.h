// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IGhostBehaviorStrategy.h"
#include "Components/SkeletalMeshComponent.h"
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
	USkeletalMesh* GhostMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	TArray<FGhostBehaviorData> BehaviorDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	float AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	float SanityDestroyRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
	TMap<FName, bool> Weakness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Data")
    TSubclassOf<class UAnimInstance> Animation;

};