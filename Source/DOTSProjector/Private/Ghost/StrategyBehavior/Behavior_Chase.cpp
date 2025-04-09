// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Chase.h"
#include "GhostBase.h" 

void UBehavior_Chase::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;

	FVector GhostLocation = Context.Ghost->GetActorLocation();
	FVector TargetLocation = Context.Target->GetActorLocation();

	FVector Direction = (TargetLocation - GhostLocation).GetSafeNormal();
	float Speed = 200.f;

	FVector NewLocation = GhostLocation + Direction * Speed * Context.Ghost->GetWorld()->GetDeltaSeconds();
	Context.Ghost->SetActorLocation(NewLocation);
}
