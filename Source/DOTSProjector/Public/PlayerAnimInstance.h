// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Item_Base.h"
#include "PlayerAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCrouching = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed = 0.f;

	UPROPERTY()
	class APhasmophobiaPlayer* Player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSettings")
	bool bHasItem = false;

	// 회전값 기억변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSettings")
	float pitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType CurrentItemType = EItemType::None;

public:
	// 초기화, 업데이트 함수
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
