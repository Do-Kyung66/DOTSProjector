// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearWidget.h"
#include "PhasmophobiaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ClearButton)
	{
		ClearButton->OnClicked.AddDynamic(this, &UClearWidget::OnClearButtonClicked);
	}
}

void UClearWidget::OnClearButtonClicked()
{


	UWorld* World = GetWorld();
	APhasmophobiaPlayerController* PlayerController = Cast<APhasmophobiaPlayerController>(UGameplayStatics::GetPlayerController(World, 0));

	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);

}
