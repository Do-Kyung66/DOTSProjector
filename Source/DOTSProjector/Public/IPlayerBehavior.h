// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "IPlayerBehavior.generated.h"

// 언리얼의 UObject 시스템에서 인식 가능한 인터페이스 타입을 선언하기 위한 틀, 리플렉션 지원/블루프린트에서 인식 가능하게 하기 위해 꼭 필요하다.
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerBehavior : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
 // 실제 인터페이스 함수 선언부, 순수 가상함수 선언은 여기에서만 가능하다.
class DOTSPROJECTOR_API IPlayerBehavior
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 공통 메서드
	// 모든 행동이 구현해야 할 순수 가상 함수 
	virtual void ExecuteBehavior(AActor* Owner, const FInputActionValue& Value) = 0;
};
