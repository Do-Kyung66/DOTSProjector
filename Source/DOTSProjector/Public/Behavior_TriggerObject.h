// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGhostBehaviorStrategy.h"
#include "Behavior_TriggerObject.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UBehavior_TriggerObject : public UGhostBehaviorStrategy
{
	GENERATED_BODY()

public:
	virtual void ExecuteBehavior(const FGhostBehaviorContext& Context) override;
};
