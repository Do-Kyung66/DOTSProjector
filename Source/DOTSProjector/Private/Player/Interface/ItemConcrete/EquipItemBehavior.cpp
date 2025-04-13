// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemBehavior.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhasmophobiaPlayer.h"
#include "PhasmophobiaPlayerController.h"

UEquipItemBehavior::UEquipItemBehavior()
{

}

void UEquipItemBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	
	auto pc = Cast<APhasmophobiaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!pc || !pc->bCanInteract) return;
	
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
	if (Player->currentItem && Player->currentItem != Player->ownedItem)
	{
		Player->currentItem->SetActorHiddenInGame(true);
	}

	if (!Player->ItemActors.Contains(Player->ownedItem))
	{
		// 0은 무시하고 1~3 슬롯 중 비어있는 곳 찾기
		for (int32 i = 1; i < Player->ItemActors.Num(); ++i)
		{
			if (Player->ItemActors[i] == nullptr)
			{
				Player->ItemActors[i] = Player->ownedItem;
				break;
			}
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

	if (UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(Player->currentItem->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		Mesh->AttachToComponent(Player->ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

