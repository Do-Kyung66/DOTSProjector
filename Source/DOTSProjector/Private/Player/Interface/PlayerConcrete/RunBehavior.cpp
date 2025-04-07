// Fill out your copyright notice in the Description page of Project Settings.


#include "RunBehavior.h"
#include "PhasmophobiaPlayer.h"

URunBehavior::URunBehavior()
{

}

void URunBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if (!Player) return;

	// 로직 구현
	// 무브 중에만 달릴 수 있음

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player Run"));
}