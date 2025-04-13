// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Observer.generated.h"

UINTERFACE(MinimalAPI)
class UObserver : public UInterface
{
	GENERATED_BODY()
};


class DOTSPROJECTOR_API IObserver
{
	GENERATED_BODY()

public:
	virtual void PlayerSanityChanged(float NewSanity) = 0;
};