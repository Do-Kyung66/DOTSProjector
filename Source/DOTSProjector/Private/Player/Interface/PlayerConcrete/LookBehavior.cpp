// Fill out your copyright notice in the Description page of Project Settings.


#include "LookBehavior.h"
#include "PhasmophobiaPlayer.h"

ULookBehavior::ULookBehavior()
{

}

void ULookBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	
	FVector2D LookInput = Value.Get<FVector2D>();

	if (Player)
	{
		Player->AddControllerYawInput(LookInput.X);
		Player->AddControllerPitchInput(LookInput.Y);
	}
}

