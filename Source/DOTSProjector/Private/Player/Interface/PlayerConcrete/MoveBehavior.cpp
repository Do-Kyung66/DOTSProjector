// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBehavior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhasmophobiaPlayer.h"

UMoveBehavior::UMoveBehavior() // 傅农 俊矾肺 积己磊 积己
{
	
}

void UMoveBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if(!Player) return;

	FVector2D InputVec = Value.Get<FVector2D>();
	FVector Forward = Player->GetActorForwardVector();
	FVector Right = Player->GetActorRightVector();

	FVector MoveDir = (Forward * InputVec.X) + (Right * InputVec.Y);

	// sound
	/*if (!MoveDir.IsNearlyZero() && !Player->bIsMoving)
	{
		Player->bIsMoving = true;
		Player->StartFootstepSound();
	}
	else if(MoveDir.IsNearlyZero() && Player->bIsMoving)
	{
		Player->bIsMoving = false;
		Player->StopFootstepSound();
	}*/
	
	if (!MoveDir.IsNearlyZero())
	{
		Player->AddMovementInput(MoveDir.GetSafeNormal(), 0.5f);
	}
	
}
