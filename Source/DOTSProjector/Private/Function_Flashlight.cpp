// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_Flashlight.h"
#include "Item_Flashlight.h"

void UFunction_Flashlight::Use(AActor* OwnerActor)
{
    if (AItem_Flashlight* Flash = Cast<AItem_Flashlight>(OwnerActor))
    {
        if (Flash->SpotLightComp)
        {
            bool bIsOn = Flash->SpotLightComp->IsVisible();
            Flash->SpotLightComp->SetVisibility(!bIsOn);
        }
    }
}
