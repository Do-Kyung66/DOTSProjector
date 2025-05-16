// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchBehavior.h"
#include "PhasmophobiaPlayer.h"
#include "Camera/CameraComponent.h"

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
		if (Player->IsLocallyControlled())
		{
			Player->CamComp->SetRelativeLocation(FVector(0.0f, 0.0f, 24.0f));
		}
	}
	else
	{
		bCrouched = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Player UnCrouch"));
		if (Player->IsLocallyControlled())
		{
			Player->CamComp->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
		}
	}

}
