// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IGhostBehaviorStrategy.generated.h"


USTRUCT(BlueprintType)
struct FGhostBehaviorContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Behavior")
	class AGhostBase* Ghost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Behavior")
	AActor* Target;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost Behavior")
	class AItem_Base* Item;
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class DOTSPROJECTOR_API UGhostBehaviorStrategy : public UObject
{
	GENERATED_BODY()

public:
	virtual void ExecuteBehavior(const FGhostBehaviorContext& Context);
};
