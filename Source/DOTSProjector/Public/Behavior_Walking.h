// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGhostBehaviorStrategy.h"
#include "Behavior_Walking.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class DOTSPROJECTOR_API UBehavior_Walking : public UGhostBehaviorStrategy
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(const FGhostBehaviorContext& Context) override;
};