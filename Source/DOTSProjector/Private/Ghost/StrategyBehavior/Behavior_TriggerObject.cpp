// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_TriggerObject.h"
#include "Item_Base.h"
#include "GhostBase.h"

void UBehavior_TriggerObject::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Item) return;

	AGhostBase* Ghost = Context.Ghost;
	AItem_Base* Item = Context.Item;

	Item->UseItem();

	Ghost->CanTrigger = false;
	Ghost->currentState = GhostState::Idle;
}
