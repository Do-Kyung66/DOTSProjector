// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Base.h"
#include "Components/SpotLightComponent.h"
#include "HouseItem_Base.h"
#include "Item_LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AItem_LightSwitch : public AItem_Base
{
	GENERATED_BODY()

public:
	AItem_LightSwitch();

	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
    USpotLightComponent* SpotLightComp;


};
