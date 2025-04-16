// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Throw.h"
#include "Item_Base.h"
#include "GhostBase.h"

void UBehavior_Throw::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (!Context.Item || !Context.Ghost) return;

	AGhostBase* Ghost = Context.Ghost;
	AItem_Base* Item = Context.Item;

	FVector GhostLocation = Ghost->GetActorLocation();
	FVector ItemLocation = Item->GetActorLocation();

	float Distance = FVector::Dist(GhostLocation, ItemLocation);

	if (Distance > 100.f)
	{

		FVector Direction = (ItemLocation - GhostLocation).GetSafeNormal();
		Ghost->SetActorLocation(GhostLocation + Direction * 300.f * GetWorld()->DeltaTimeSeconds);
		return;
	}

	Item->SetActorHiddenInGame(true);
	Item->SetActorEnableCollision(false);
	Item->AttachToActor(Ghost, FAttachmentTransformRules::KeepWorldTransform);

	FTimerHandle TimerHandle;
	Ghost->GetWorldTimerManager().SetTimer(TimerHandle, [Ghost, Item]() {

		FVector ThrowDirection = Ghost->GetActorForwardVector();
		Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Item->SetActorHiddenInGame(false);
		Item->SetActorEnableCollision(true);


		if (UPrimitiveComponent* ItemRoot = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
		{
			ItemRoot->SetSimulatePhysics(true);
			ItemRoot->AddImpulse(ThrowDirection * 1000.f);
		}

		}, 1.0f, false);
	
	Ghost->currentState = GhostState::Idle;
}
