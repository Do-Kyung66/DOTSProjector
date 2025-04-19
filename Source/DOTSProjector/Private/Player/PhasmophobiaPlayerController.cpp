// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayerController.h"
#include "Engine/UserInterfaceSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"


APhasmophobiaPlayerController::APhasmophobiaPlayerController()
{

	//bShowMouseCursor = true;
	bEnableClickEvents = true;
	//bEnableMouseOverEvents = true;

	//DefaultMouseCursor = EMouseCursor::Default;

}

void APhasmophobiaPlayerController::BeginPlay()
{
	


}

void APhasmophobiaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	ItemTrace();


}

void APhasmophobiaPlayerController::SetCursorForInteraction(bool bIsInteractable, AActor* tempItem)
{

	if (bIsInteractable)
	{
		bCanInteract = true;
		TargetItem = tempItem;
		//CurrentMouseCursor = EMouseCursor::Hand;
		UE_LOG(LogTemp, Warning, TEXT("Cursor : Hand"));
	}
	else
	{
		bCanInteract = false;
		TargetItem = nullptr;
		//CurrentMouseCursor = EMouseCursor::Default;
		UE_LOG(LogTemp, Warning, TEXT("Cursor : Default"));
	}


	/*APhasmophobiaHUD* HUD = Cast<APhasmophobiaHUD>(GetHUD());
	if (HUD && HUD->bIsJournalOpen) return;*/

	//if (bIsInteractable)
	//{
	//	bCanInteract = true;
	//	TargetItem = tempItem;
	//	CurrentMouseCursor = EMouseCursor::Hand;
	//	UE_LOG(LogTemp, Warning, TEXT("Cursor : Hand"));
	//}
	//else
	//{
	//	bCanInteract = false;
	//	TargetItem = nullptr;
	//	CurrentMouseCursor = EMouseCursor::Default;
	//	UE_LOG(LogTemp, Warning, TEXT("Cursor : Default"));
	//}

	//FInputModeGameAndUI InputMode;
	
}

void APhasmophobiaPlayerController::ItemTrace()
{
	FVector worldLocation;
	FVector worldDirection;

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D screenCenter(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);
	DeprojectScreenPositionToWorld(screenCenter.X, screenCenter.Y, worldLocation, worldDirection);

	FVector Start = worldLocation;
	FVector End = Start + (worldDirection * 300.0f);

	FHitResult Hitinfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(Hitinfo, Start, End, ECC_Visibility, params))
	{
		GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *Hitinfo.GetActor()->GetName()));

		AActor* HitActor = Hitinfo.GetActor();
		if (HitActor)
		{
			FString ActorName = HitActor->GetName();

			if (ActorName.Contains(TEXT("item"), ESearchCase::IgnoreCase)) // 대소문자 무시
			{
				bCanInteract = true;
				TargetItem = HitActor;
				UE_LOG(LogTemp, Warning, TEXT("Hit Item"));

			}
			else
			{
				bCanInteract = false;
				TargetItem = nullptr;
				UE_LOG(LogTemp, Warning, TEXT("Item X"));
			}
		}
		
	}
	else
	{
		bCanInteract = false;
		TargetItem = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Nothing Hit"));
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.05f, 0, 2.0f);

}

