// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PhasmophobiaPlayer.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Player = Cast<APhasmophobiaPlayer>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Player)
	{
		speed = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		direction = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
	}
}
