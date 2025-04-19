// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Kill.h"
#include "PhasmophobiaPlayer.h"

void UBehavior_Kill::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (APhasmophobiaPlayer* PlayerCharacter = Cast<APhasmophobiaPlayer>(Context.Target)) {
		PlayerCharacter->IsDead = true;
	}
}
