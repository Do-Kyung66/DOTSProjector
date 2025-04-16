// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStrategy.h"
#include "Function_Crucifix.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UFunction_Crucifix : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(AItem_Base* Item) override;
};
