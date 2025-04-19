// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// ���� �ν��Ͻ� �ʱ�ȭ �Լ�(���� ���� �� �ڵ� ȣ���)
	virtual void Init() override;

public:
	// ���� �������̽� ������ (�¶��� ����ý����� ���� ���� ��ɿ� ������ �� ����)
	IOnlineSessionPtr sessionInterface;

	// ���� ���� �Լ�(�� �̸��� �ִ� �ο����� 4������ �����̶� ���� ����� ���η� �� ����)
	void CreateMySession(FString roomName, bool bIsPrivate);

	// ���� ȣ��Ʈ �̸�
	FString mySessionName = "test";
	
	// ���� ���� �Ϸ� �� ȣ��Ǵ� �ݹ� �Լ�
	// ������ ���� �̸�, ���� ���� ���� ����
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
};
