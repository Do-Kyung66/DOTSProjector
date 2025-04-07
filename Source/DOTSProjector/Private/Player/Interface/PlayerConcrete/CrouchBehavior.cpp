// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchBehavior.h"
#include "PhasmophobiaPlayer.h"

UCrouchBehavior::UCrouchBehavior()
{

}

void UCrouchBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if(!Player) return;

	// 로직 구현
	/*if (Player->bIsCrouched)
	{
		Player->UnCrouch();
	}
	else
	{
		Player->Crouch();
	}*/

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Player Crouch"));

}

// 0407 input 값 부모단에서 늘려서 상속 받기
//void UCrouchBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value, EInputEventType InputType)
//{
//	if (InputType == EInputEventType::Started)
//		Player->Crouch();
//	else if (InputType == EInputEventType::Completed)
//		Player->UnCrouch();
//}