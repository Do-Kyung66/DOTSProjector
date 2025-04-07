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

	// ���� ����
	// ���� �߿��� �޸� �� ����

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player Run"));
}