// Fill out your copyright notice in the Description page of Project Settings.


#include "RunBehavior.h"
#include "PhasmophobiaPlayer.h"

URunBehavior::URunBehavior()
{

}

void URunBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	APhasmophobiaPlayer* Player = Cast<APhasmophobiaPlayer>(Owner);
	if (!Player) return;


	FVector CurrentVelocity = Player->GetVelocity();
	
	if (CurrentVelocity.Size() > 0 && Player->CurrentStamina > 0)
	{
		Player->bIsRunning = true;
		Player->AddMovementInput(CurrentVelocity.GetSafeNormal(), 2.0f);

		// Stamina
		Player->CurrentStamina -= Player->StaminaDrainRate * GetWorld()->GetDeltaSeconds();
		Player->CurrentStamina = FMath::Clamp(Player->CurrentStamina, 0.0f, Player->MaxStamina);
		GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Yellow, TEXT("Player Run"));
	}

		
}