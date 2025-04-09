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
	bool bIsMoving = !CurrentVelocity.IsNearlyZero(1.0f);
	FVector Forward = Player->GetActorForwardVector();
	FVector Right = Player->GetActorRightVector();


	FVector2D InputVec = Value.Get<FVector2D>();
	FVector MoveDir = (Forward * InputVec.X) + (Right * InputVec.Y);
	
	if (bIsMoving && Player->CurrentStamina > 0)
	{
		Player->bIsRunning = true;
		Player->AddMovementInput(MoveDir.GetSafeNormal(), 2.0f);

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("MoveDir: %s"), *MoveDir.ToString()));

		// Stamina
		Player->CurrentStamina -= Player->StaminaDrainRate * GetWorld()->GetDeltaSeconds();
		Player->CurrentStamina = FMath::Clamp(Player->CurrentStamina, 0.0f, Player->MaxStamina);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player Run"));
	}
	
}