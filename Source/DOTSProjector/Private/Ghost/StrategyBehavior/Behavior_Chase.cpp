// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Chase.h"
#include "GhostBase.h" 
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Kismet/GameplayStatics.h"

void UBehavior_Chase::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Ghost || !Context.Target) return;

	const FVector TargetLocation = Context.Target->GetActorLocation();

	const FVector GhostLocation = Context.Ghost->GetActorLocation();
	const FVector Direction = (TargetLocation - GhostLocation).GetSafeNormal();
	const float Speed = 200.f;

	const FVector NewLocation = GhostLocation + Direction * Speed * Context.Ghost->GetWorld()->GetDeltaSeconds();
	Context.Ghost->SetActorLocation(NewLocation);
}