// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Idle.h"

void UBehavior_Idle::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;
}
