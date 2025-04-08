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
	FVector VelocityDir = CurrentVelocity.GetSafeNormal(); // 속도 방향
	bool bIsMoving = !CurrentVelocity.IsNearlyZero(1.0f);
	FVector Forward = Player->GetActorForwardVector();
	float DotResult = FVector::DotProduct(Forward, VelocityDir);

	FVector2D InputVec = Value.Get<FVector2D>();
	
	if (DotResult > 0.7f && bIsMoving && Player->CurrentStamina > 0)
	{
		Player->bIsRunning = true;
		Player->AddMovementInput(Forward, 2.0f);

		// Stamina
		Player->CurrentStamina -= Player->StaminaDrainRate * GetWorld()->GetDeltaSeconds();
		Player->CurrentStamina = FMath::Clamp(Player->CurrentStamina, 0.0f, Player->MaxStamina);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player Run"));
	}
	else
	{
		Player->CurrentStamina += Player->StaminaRegenRate * GetWorld()->DeltaTimeSeconds;
		Player->CurrentStamina = FMath::Clamp(Player->CurrentStamina, 0.0f, Player->MaxStamina);
	}
}