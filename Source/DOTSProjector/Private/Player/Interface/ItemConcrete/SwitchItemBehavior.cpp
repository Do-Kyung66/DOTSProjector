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

	Player->ScrollValue = Value.Get<float>();

	if (Player->currentItem)
	{
		Player->currentItem->SetActorHiddenInGame(true);
	}

	// ½½·Ô ¼øÈ¯¿ë ÀÎµ¦½º
	Player->StartIndex = Player->CurrentItemIndex;
	Player->NextIndex = Player->StartIndex;


	if (Player->ScrollValue > 0.f)
	{
		// GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, FString::Printf(TEXT("ScrollValue : %.2f"), Player->ScrollValue));
		do {
			Player->NextIndex = (Player->NextIndex + 1) % Player->ItemActors.Num();
		} while (Player->NextIndex != Player->StartIndex && Player->NextIndex != 0 && Player->ItemActors[Player->NextIndex] == nullptr);
		//Player->CurrentItemIndex = (Player->CurrentItemIndex + 1) % Player->ItemActors.Num();
	}

	else if (Player->ScrollValue < 0.f)
	{
		// GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, FString::Printf(TEXT("ScrollValue : %.2f"), Player->ScrollValue));
		do {
			Player->NextIndex = (Player->NextIndex - 1 + Player->ItemActors.Num()) % Player->ItemActors.Num();
		} while (Player->NextIndex != Player->StartIndex && Player->NextIndex != 0 && Player->ItemActors[Player->NextIndex] == nullptr);
		// Player->CurrentItemIndex = (Player->CurrentItemIndex - 1 + Player->ItemActors.Num()) % Player->ItemActors.Num();
	}

	Player->CurrentItemIndex = Player->NextIndex;
	Player->currentItem = Player->ItemActors[Player->NextIndex];

	if (Player->currentItem)
	{
		Player->currentItem->SetActorHiddenInGame(false);
		Player->currentItem->AttachToComponent(Player->ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		UE_LOG(LogTemp, Log, TEXT("Switched to Item: %s (Index: %d)"), *Player->currentItem->GetName(), Player->NextIndex);
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
