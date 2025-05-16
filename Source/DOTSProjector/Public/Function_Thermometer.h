// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStrategy.h"
#include "Function_Thermometer.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UFunction_Thermometer : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(AItem_Base* Item) override;

	void Tick(float DeltaTime, class AItem_Thermometer* Thermometer);

	UPROPERTY(Replicated)
	float Temperature = 20.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
