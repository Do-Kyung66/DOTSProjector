// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingSpeed;
	UPROPERTY(BlueprintReadOnly)
	int32 index; // �˻��� ������ �ε���
	UPROPERTY(BlueprintReadOnly)
	bool bIsPrivate = false;
	UPROPERTY(BlueprintReadOnly)
	FString roomCode = "";  //������ 6�ڸ� ���ڷ� ��������� �ۺ����� ���� ����������� ���� X roomcode = IsEmpty()�� üũ
	
	FSessionInfo():roomName(""),hostName(""), playerCount(""), pingSpeed(0), index(0), bIsPrivate(false), roomCode("")
	{
	}

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s, %dms, %s"), index, *roomName, *hostName, *playerCount, pingSpeed, *roomCode);
	}
};

// ���� �˻��� ������ �� ȣ��Ǵ� ��������Ʈ
// ��������Ʈ ��ũ�η� FSessionInfo ���ڷ� �޴� FSearchSignature �̸��� ���ο� �̺�Ʈ Ÿ���� �����. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);

// ���� �˻� ���� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

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
	//UPROPERTY(BlueprintReadWrite)
	//FSessionInfo sessionInfo;

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

	// ��˻�
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	
	void FindOtherSession();

	void OnFindSessionComplete(bool bWasSuccessful);

	// ��ã�� �Ϸ� �ݹ��� ����� ��������Ʈ
	FSearchSignature onSearchCompleted;
	// ��ã����� �ݹ� ��������Ʈ
	FSearchStateSignature onSearchState;

	//���ǹ� ����
	void JoinSelectedSession(int32& index);
	void JoinPrivateRoom(FString& Code);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
};
