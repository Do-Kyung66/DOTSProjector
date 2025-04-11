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

	// 방금 장착한 아이템만 보이기
	if (Player->currentItem && Player->currentItem != Player->ownedItem)
	{
		Player->currentItem->SetActorHiddenInGame(true);
	}

	Player->ownedItem->SetActorHiddenInGame(false);
	Player->currentItem = Player->ownedItem;

	if (UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(Player->currentItem->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		Mesh->AttachToComponent(Player->ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

