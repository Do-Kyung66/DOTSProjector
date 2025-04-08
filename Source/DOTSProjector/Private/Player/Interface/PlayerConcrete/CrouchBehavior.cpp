// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchBehavior.h"
#include "PhasmophobiaPlayer.h"

UCrouchBehavior::UCrouchBehavior()
{
	bCrouched = false;
}

void UCrouchBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if(!Player) return;

	if (!bCrouched)
	{

		bCrouched = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Player Crouch"));
	}
	else
	{
		bCrouched = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Player UnCrouch"));
	}

}
