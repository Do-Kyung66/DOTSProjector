// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Base.h"
#include "Item_Thermometer.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AItem_Thermometer : public AItem_Base
{
	GENERATED_BODY()

public:
	AItem_Thermometer();

	virtual void BeginPlay() override;

};
