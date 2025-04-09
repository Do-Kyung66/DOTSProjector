// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;

	UPROPERTY()
	class APhasmophobiaPlayer* Player;

public:
	// �ʱ�ȭ, ������Ʈ �Լ�
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
