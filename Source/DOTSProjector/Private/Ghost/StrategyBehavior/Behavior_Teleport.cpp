// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Teleport.h"

void UBehavior_Teleport::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;
}
