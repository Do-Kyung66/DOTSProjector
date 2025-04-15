// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "IItemBehavior.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemBehavior : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOTSPROJECTOR_API IItemBehavior
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 공통 메서드
	// 모든 행동이 구현해야 할 순수 가상 함수 
	virtual void ExecuteBehavior(AActor* Owner, const FInputActionValue& Value) = 0;
};
