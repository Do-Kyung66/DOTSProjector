// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IPlayerBehavior.h"
#include "MoveBehavior.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UMoveBehavior : public UObject, public IPlayerBehavior
{
	GENERATED_BODY()

public:
	UMoveBehavior();

	UFUNCTION()
	virtual void ExecuteBehavior(AActor* Owner, const FInputActionValue& Value) override;

	
};
