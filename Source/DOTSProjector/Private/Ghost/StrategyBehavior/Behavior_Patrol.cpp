// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Patrol.h"
#include "GhostBase.h" 

void UBehavior_Patrol::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost) return;

	float DistanceToTarget = FVector::Dist(Context.Ghost->GetActorLocation(), Context.Ghost->RandomPos);

	Context.Ghost->GetCharacterMovement()->MaxWalkSpeed = 100.f;

	if (DistanceToTarget < 100.f)
	{
		if (Context.Ghost->GetRandomPositionInNavMesh(Context.Ghost->GetActorLocation(), 800.f, Context.Ghost->RandomPos))
		{
			AAIController* AIController = Cast<AAIController>(Context.Ghost->GetController());
			AIController->MoveToLocation(Context.Ghost->RandomPos);
		}
	}
}
