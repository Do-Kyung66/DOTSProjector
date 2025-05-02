// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStrategy.h"
#include "Function_VideoCamera.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UFunction_VideoCamera : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(AItem_Base* Item) override;


};
