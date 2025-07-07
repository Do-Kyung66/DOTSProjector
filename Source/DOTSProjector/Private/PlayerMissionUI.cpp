// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMissionUI.h"
#include "Components/Image.h"
#include "GameFramework/PlayerState.h"
#include "PhasmophobiaPlayer.h"
#include "GameFramework/GameStateBase.h"
#include "Item_VideoCamera.h"

void UPlayerMissionUI::NativeConstruct()
{
    Super::NativeConstruct();

    Image_CheckSanity->SetVisibility(ESlateVisibility::Hidden);
	Image_CheckEscape->SetVisibility(ESlateVisibility::Hidden);
	Image_CheckCamera->SetVisibility(ESlateVisibility::Hidden);
}


void UPlayerMissionUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

   playerArr = GetWorld()->GetGameState()->PlayerArray;

    for (APlayerState* pState : playerArr)
    {
        if (APhasmophobiaPlayer* PlayerPawn = Cast<APhasmophobiaPlayer>(pState->GetPawn()))
        {
            if (PlayerPawn->Sanity <= 25 && !SanityMission) {
                SanityMission = true;
                Image_CheckSanity->SetVisibility(ESlateVisibility::Visible);
                PlayerPawn->MissionCount++;
            }
			AItem_VideoCamera* VC = Cast<AItem_VideoCamera>(PlayerPawn->currentItem);
			if (VC && VC->CaptureGhost && !PictureMission) {
                PictureMission = true;
                PlayerPawn->MissionCount++;
				Image_CheckCamera->SetVisibility(ESlateVisibility::Visible);
			}
        }
    }
}
