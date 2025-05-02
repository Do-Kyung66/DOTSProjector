// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Walking.h"
#include "GhostBase.h" 


void UBehavior_Walking::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;

	Context.Ghost->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	AAIController* AIController = Cast<AAIController>(Context.Ghost->GetController());

	AIController->MoveToActor(Context.Target, 100.0f);
}

