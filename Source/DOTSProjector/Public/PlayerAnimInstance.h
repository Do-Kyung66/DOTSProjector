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
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float direction;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool bIsDead = false;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY()
	class APhasmophobiaPlayer* Player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSettings")
	bool bHasItem = false;

	// ȸ���� ��ﺯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSettings")
	float pitchAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType CurrentItemType = EItemType::None;

	UFUNCTION()
	void AnimNotify_DieEnd();

public:
	// �ʱ�ȭ, ������Ʈ �Լ�
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
