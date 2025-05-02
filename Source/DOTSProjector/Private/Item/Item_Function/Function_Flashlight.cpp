// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_Flashlight.h"
#include "Item_Flashlight.h"

void UFunction_Flashlight::Use(AItem_Base* Item)
{
    if (AItem_Flashlight* Flash = Cast<AItem_Flashlight>(Item))
    {
        Flash->bIsOn = !Flash->bIsOn;
        Flash->OnRep_LightState();
    }
}


