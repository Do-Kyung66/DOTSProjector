// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_LightSwitch.h"
#include "Item_LightSwitch.h"

	
void UFunction_LightSwitch::Use(AItem_Base* Item)
{
	if (AItem_LightSwitch* LightSwitch = Cast<AItem_LightSwitch>(Item)) {
		
		if (LightSwitch->SpotLightComp)
		{
			bOn = !LightSwitch->SpotLightComp->IsVisible();

			if (bOn) {
				LightSwitch->SpotLightComp->SetVisibility(bOn);
			}
			else {
				LightSwitch->SpotLightComp->SetVisibility(bOn);
			}
		}
	}
}	
