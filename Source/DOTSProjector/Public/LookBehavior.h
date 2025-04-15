// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IPlayerBehavior.h"
#include "LookBehavior.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API ULookBehavior : public UObject, public IPlayerBehavior
{
	GENERATED_BODY()

public:
	ULookBehavior();

	UFUNCTION()
	virtual void ExecuteBehavior(AActor* Owner, const FInputActionValue& Value) override;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
	class APhasmophobiaPlayerController* PC;
};
