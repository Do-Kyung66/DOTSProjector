// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_VideoCamera.h"
#include "Item_VideoCamera.h"
#include "PhasmophobiaPlayer.h"

void UFunction_VideoCamera::Use(AItem_Base* Item)
{
    if (AItem_VideoCamera* VC = Cast<AItem_VideoCamera>(Item))
    {
        VC->bIsOn = !VC->bIsOn;
        VC->OnRep_VideoState();

        APhasmophobiaPlayer* PlayerCharacter = Cast< APhasmophobiaPlayer>(Item->GetOwner());
        if (PlayerCharacter) {
            if (PlayerCharacter->SawGhost && VC->bIsOn) {
                VC->CaptureGhost = true;
            }
        }
    }
}
