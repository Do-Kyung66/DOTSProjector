// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchItemBehavior.h"
#include "PhasmophobiaPlayer.h"

USwitchItemBehavior::USwitchItemBehavior()
{

}

void USwitchItemBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if(!Player || Player->ItemActors.IsEmpty()) return;

	UE_LOG(LogTemp, Log, TEXT("Switch Switch"));

	float ScrollValue = Value.Get<float>();

	if (Player->currentItem)
	{
		Player->currentItem->SetActorHiddenInGame(true);
	}

	// ½½·Ô ¼øÈ¯¿ë ÀÎµ¦½º
	int32 StartIndex = Player->CurrentItemIndex;
	int32 NextIndex = StartIndex;


	if (ScrollValue > 0.f)
	{
		do {
			NextIndex = (NextIndex + 1) % Player->ItemActors.Num();
		} while (NextIndex != StartIndex && NextIndex != 0 && Player->ItemActors[NextIndex] == nullptr);
		//Player->CurrentItemIndex = (Player->CurrentItemIndex + 1) % Player->ItemActors.Num();
	}

	else if (ScrollValue < 0.f)
	{
		do {
			NextIndex = (NextIndex - 1 + Player->ItemActors.Num()) % Player->ItemActors.Num();
		} while (NextIndex != StartIndex && NextIndex != 0 && Player->ItemActors[NextIndex] == nullptr);
		// Player->CurrentItemIndex = (Player->CurrentItemIndex - 1 + Player->ItemActors.Num()) % Player->ItemActors.Num();
	}

	Player->CurrentItemIndex = NextIndex;
	Player->currentItem = Player->ItemActors[NextIndex];

	if (Player->currentItem)
	{
		Player->currentItem->SetActorHiddenInGame(false);
		Player->currentItem->AttachToComponent(Player->ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		UE_LOG(LogTemp, Log, TEXT("Switched to Item: %s (Index: %d)"), *Player->currentItem->GetName(), NextIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Switched to Empty Slot (Index: 0)"));
	}

	/*if (Player->ItemActors.IsValidIndex(Player->CurrentItemIndex))
	{
		Player->currentItem = Player->ItemActors[Player->CurrentItemIndex];
		if (Player->currentItem)
		{
			Player->currentItem->SetActorHiddenInGame(false);
		}
	}*/
}
