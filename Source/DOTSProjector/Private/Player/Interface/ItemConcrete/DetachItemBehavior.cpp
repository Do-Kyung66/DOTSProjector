// Fill out your copyright notice in the Description page of Project Settings.


#include "DetachItemBehavior.h"
#include "PhasmophobiaPlayer.h"

UDetachItemBehavior::UDetachItemBehavior()
{

}

void UDetachItemBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Detach!"));
	
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);

	if(!Player->bHasItem) return;

	if (Player->currentItem)
	{
		
		if (UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(Player->currentItem->GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		}
		
		Player->ItemActors[Player->CurrentItemIndex] = nullptr;
		Player->currentItem->SetOwner(nullptr);
		Player->currentItem = nullptr;
		Player->CurrentItemIndex = Player->CurrentItemIndex+1;
	}
}
