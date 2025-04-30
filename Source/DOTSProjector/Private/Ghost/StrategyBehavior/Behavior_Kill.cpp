// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior_Kill.h"
#include "PhasmophobiaPlayer.h"
#include "Item_Crucifix.h"

void UBehavior_Kill::ExecuteBehavior(const FGhostBehaviorContext& Context)
{
	if (APhasmophobiaPlayer* PlayerCharacter = Cast<APhasmophobiaPlayer>(Context.Target)) {
		if (PlayerCharacter->currentItem) {
			if (Cast<AItem_Crucifix>(PlayerCharacter->currentItem)) {
				PlayerCharacter->currentItem->Destroy();
				PlayerCharacter->ItemActors[PlayerCharacter->CurrentItemIndex] = nullptr;
				PlayerCharacter->currentItem->SetOwner(nullptr);
				PlayerCharacter->currentItem = nullptr;
				PlayerCharacter->CurrentItemIndex = 0;
				return;
			}
		}
		PlayerCharacter->ServerRPC_Detach();

		for (auto& item : PlayerCharacter->ItemActors) {
			PlayerCharacter->currentItem = item;
			PlayerCharacter->ServerRPC_Detach();
		}
		PlayerCharacter->bIsDead = true;
	}
}