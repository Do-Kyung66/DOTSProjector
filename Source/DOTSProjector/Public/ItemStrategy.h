// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStrategy.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UItemStrategy : public UObject
{
	GENERATED_BODY()

public:
	virtual void Use(class AItem_Base* Item) {};
};
