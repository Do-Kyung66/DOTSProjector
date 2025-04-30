// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PhasmophobiaPlayer.h"
#include "Net/UnrealNetwork.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APhasmophobiaPlayer>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		speed = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());
		direction = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());

		// 회전값 적용
		pitchAngle = -Player->GetBaseAimRotation().GetNormalized().Pitch;
		pitchAngle = FMath::Clamp(pitchAngle, -60.0f, 60.0f);

		// 사망 여부 
		isDead = Player->bIsDead;

		// 아이템 소유 여부
		bHasItem = Player->bHasItem;

		if (Player->currentItem)
		{
			if (Player->currentItem->GetName().Contains(TEXT("Flashlight"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::Flashlight;
				//UE_LOG(LogTemp, Log, TEXT("Flashlight state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("Crucifix"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::Crucifix;
				//UE_LOG(LogTemp, Log, TEXT("Crucifix state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("DotProjector"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::DotProjector;
				//UE_LOG(LogTemp, Log, TEXT("DotProjector state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("EMFReader"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::EMFReader;
				//UE_LOG(LogTemp, Log, TEXT("EMFReader state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("UVLight"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::UVLight;
				//UE_LOG(LogTemp, Log, TEXT("UVLight state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("Thermometer"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::Thermometer;
				//UE_LOG(LogTemp, Log, TEXT("Thermometer state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("VideoCamera"), ESearchCase::IgnoreCase))
			{
				Player->CurrentItemType = EItemType::VideoCamera;
				//UE_LOG(LogTemp, Log, TEXT("VideoCamera state"));
			}
		}
		else
		{
			Player->CurrentItemType = EItemType::None;
			//UE_LOG(LogTemp, Log, TEXT("None state"));
		}
		CurrentItemType = Player->CurrentItemType;
	
	}
}

void UPlayerAnimInstance::AnimNotify_DieEnd()
{
	// 조종을 하는 유저 화면에서만 DieProcess 실행
	if (Player && Player->IsLocallyControlled())
	{
		Player->DieProcess();
	}
}