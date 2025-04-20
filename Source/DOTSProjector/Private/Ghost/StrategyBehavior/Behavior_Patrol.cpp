// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Patrol.h"
#include "GhostBase.h" 

void UBehavior_Patrol::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost) return;

	Context.Ghost->GetCharacterMovement()->MaxWalkSpeed = 100.f;

	if (!Context.Ghost->bPatrol){
		Context.Ghost->bPatrol = true;
		if (Context.Ghost->GetRandomPositionInNavMesh(Context.Ghost->GetActorLocation(), 800.f, Context.Ghost->RandomPos))
		{
			AAIController* AIController = Cast<AAIController>(Context.Ghost->GetController());
			//GEngine->AddOnScreenDebugMessage(20, 1.f, FColor::Green, TEXT("Patrol working"));
			AIController->MoveToLocation(Context.Ghost->RandomPos);
		}
	}
}
