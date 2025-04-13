// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IItemBehavior.h"
#include "SwitchItemBehavior.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API USwitchItemBehavior : public UObject, public IItemBehavior
{
	GENERATED_BODY()

public:
	USwitchItemBehavior();

	UFUNCTION()
	virtual void ExecuteBehavior(AActor* Owner, const FInputActionValue& Value) override;
	
};
