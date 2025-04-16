// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PhasmophobiaPlayer.h"

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

		// 회전값 적용
		pitchAngle = -Player->GetBaseAimRotation().GetNormalized().Pitch;
		pitchAngle = FMath::Clamp(pitchAngle, -60.0f, 60.0f);

		// 아이템 소유 여부
		bHasItem = Player->bHasItem;

		// 들고 있는 아이템 업데이트
		if (Player->currentItem)
		{
			if (Player->currentItem->GetName().Contains(TEXT("Flashlight"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::Flashlight;
				UE_LOG(LogTemp, Log, TEXT("Flashlight state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("Crucifix"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::Crucifix;
				UE_LOG(LogTemp, Log, TEXT("Crucifix state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("DotProjector"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::DotProjector;
				UE_LOG(LogTemp, Log, TEXT("DotProjector state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("EMFReader"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::EMFReader;
				UE_LOG(LogTemp, Log, TEXT("EMFReader state"));

			}
			else if (Player->currentItem->GetName().Contains(TEXT("UVLight"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::UVLight;
				UE_LOG(LogTemp, Log, TEXT("UVLight state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("Thermometer"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::Thermometer;
				UE_LOG(LogTemp, Log, TEXT("Thermometer state"));
			}
			else if (Player->currentItem->GetName().Contains(TEXT("VideoCamera"), ESearchCase::IgnoreCase))
			{
				CurrentItemType = EItemType::VideoCamera;
				UE_LOG(LogTemp, Log, TEXT("VideoCamera state"));
			}
		}
		else
		{
			CurrentItemType = EItemType::None;
			UE_LOG(LogTemp, Log, TEXT("None state"));
		}
		
	}
}
