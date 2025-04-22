// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemBehavior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhasmophobiaPlayer.h"
#include "PhasmophobiaPlayerController.h"
#include "PlayerAnimInstance.h"

UEquipItemBehavior::UEquipItemBehavior()
{

}

void UEquipItemBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	
	/*auto pc = Cast<APhasmophobiaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!pc || !pc->bCanInteract) return;*/
	
	/*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("EquipItem"));*/
	UE_LOG(LogTemp, Warning, TEXT("EquipItem"));

	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if (!Player) return;

	// 장착 전에 아이템 배열 로그 찍기
	for (int32 i = 0; i < Player->ItemActors.Num(); ++i)
	{
		if (Player->ItemActors[i])
		{
			UE_LOG(LogTemp, Log, TEXT("ItemActors[%d]: %s"), i, *Player->ItemActors[i]->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ItemActors[%d]: nullptr"), i);
		}
	}

	// 방금 장착한 아이템만 보이기
	//if (Player->currentItem && Player->currentItem != Player->ownedItem)
	//{
	//	//Player->currentItem->SetActorHiddenInGame(true);
	//	Player->ownedItem->SetActorHiddenInGame(true);

	//}

	if (!Player->ItemActors.Contains(Player->ownedItem))
	{
		bool bInserted = false;
		// 0은 무시하고 1~3 슬롯 중 비어있는 곳 찾기
		for (int32 i = 1; i < Player->ItemActors.Num(); ++i)
		{
			if (Player->ItemActors[i] == nullptr)
			{
				Player->ItemActors[i] = Player->ownedItem;
				bInserted = true;
				break;
			}
		}
		if(!bInserted) return;
	}

	// 장착 후에 아이템 배열 로그 찍기
	for (int32 i = 0; i < Player->ItemActors.Num(); ++i)
	{
		if (Player->ItemActors[i])
		{
			UE_LOG(LogTemp, Log, TEXT("Equip after ItemActors[%d]: %s"), i, *Player->ItemActors[i]->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Equip after ItemActors[%d]: nullptr"), i);
		}
	}

	// 이전 아이템 숨기기
	if (Player->currentItem && Player->currentItem != Player->ownedItem)
	{
		Player->currentItem->SetActorHiddenInGame(true);
	}
	Player->ownedItem->SetActorHiddenInGame(false);
	Player->currentItem = Player->ownedItem;
	UE_LOG(LogTemp, Log, TEXT("CurrentItem: %s"), *Player->currentItem->GetName());
	Player->CurrentItemIndex = Player->ItemActors.Find(Player->currentItem);
	UE_LOG(LogTemp, Log, TEXT("Index in ItemActors: %d"), Player->CurrentItemIndex);

	// 아이템마다 애니메이션 교체
	/*AnimInstance = Cast<UPlayerAnimInstance>(Player->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		if (Player->currentItem->GetName().Contains(TEXT("Flashlight"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::Flashlight;
			UE_LOG(LogTemp, Log, TEXT("Flashlight state"));
		}
		else if (Player->currentItem->GetName().Contains(TEXT("Crucifix"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::Crucifix;
			UE_LOG(LogTemp, Log, TEXT("Crucifix state"));
		}
		else if (Player->currentItem->GetName().Contains(TEXT("DotProjector"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::DotProjector;
			UE_LOG(LogTemp, Log, TEXT("DotProjector state"));
		}
		else if (Player->currentItem->GetName().Contains(TEXT("EMFReader"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::EMFReader;
			UE_LOG(LogTemp, Log, TEXT("EMFReader state"));

		}
		else if (Player->currentItem->GetName().Contains(TEXT("UVLight"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::UVLight;
			UE_LOG(LogTemp, Log, TEXT("UVLight state"));
		}
		else if (Player->currentItem->GetName().Contains(TEXT("Thermometer"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::Thermometer;
			UE_LOG(LogTemp, Log, TEXT("Thermometer state"));
		}
		else if (Player->currentItem->GetName().Contains(TEXT("VideoCamera"), ESearchCase::IgnoreCase))
		{
			AnimInstance->CurrentItemType = EItemType::VideoCamera;
			UE_LOG(LogTemp, Log, TEXT("VideoCamera state"));
		}
	}*/
	

	if (UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(Player->currentItem->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		/*Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		Mesh->AttachToComponent(Player->ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);*/

		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		if (Player->currentItem->GetName().Contains(TEXT("Flashlight")))
		{
			FName SocketName = TEXT("FlashlightSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"),*SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("Crucifix")))
		{
			FName SocketName = TEXT("CrucifixSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("DotProjector")))
		{
			FName SocketName = TEXT("DotProjectorSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("EMFReader")))
		{
			FName SocketName = TEXT("EMFReaderSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("UVLight")))
		{
			FName SocketName = TEXT("UVLightSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("Thermometer")))
		{
			FName SocketName = TEXT("ThermometerSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
		else if (Player->currentItem->GetName().Contains(TEXT("VideoCamera")))
		{
			FName SocketName = TEXT("VideoCameraSocket");
			Mesh->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			UE_LOG(LogTemp, Log, TEXT("SoketName : %s"), *SocketName.ToString());
		}
	}
}

