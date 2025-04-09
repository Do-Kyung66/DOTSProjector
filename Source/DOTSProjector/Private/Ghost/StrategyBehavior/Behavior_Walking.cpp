// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Walking.h"
#include "AIController.h"
#include "GhostBase.h" 
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBehavior_Walking::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;

	Context.Ghost->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	AAIController* AIController = Cast<AAIController>(Context.Ghost->GetController());

	AIController->MoveToActor(Context.Target, 100.0f);
}

