// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStrategy.h"
#include "Function_EMFReader.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UFunction_EMFReader : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(AItem_Base* Item) override;
	void Tick(float DeltaTime, class AItem_EMFReader* EMFReader);
	bool bOn = false;
};
