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
	int32 index; // 검색된 섹션의 인덱스
	UPROPERTY(BlueprintReadOnly)
	bool bIsPrivate = false;
	UPROPERTY(BlueprintReadOnly)
	FString roomCode = "";  //랜덤한 6자리 숫자로 만들어지고 퍼블릭으로 방이 만들어질때는 값이 X roomcode = IsEmpty()로 체크
	
	FSessionInfo():roomName(""),hostName(""), playerCount(""), pingSpeed(0), index(0), bIsPrivate(false), roomCode("")
	{
	}

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s, %dms, %s"), index, *roomName, *hostName, *playerCount, pingSpeed, *roomCode);
	}
};

// 세션 검색이 끝났을 때 호출되는 델리게이트
// 델리게이트 매크로로 FSessionInfo 인자로 받는 FSearchSignature 이름의 새로운 이벤트 타입을 만든다. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);

// 세션 검색 상태 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 게임 인스턴스 초기화 함수(게임 시작 시 자동 호출됨)
	virtual void Init() override;

public:
	//UPROPERTY(BlueprintReadWrite)
	//FSessionInfo sessionInfo;

	// 세션 인터페이스 포인터 (온라인 서브시스템을 통해 세션 기능에 접근할 수 있음)
	IOnlineSessionPtr sessionInterface;

	// 세션 생성 함수(방 이름과 최대 인원수는 4명으로 고정이라 공개 비공개 여부로 방 생성)
	void CreateMySession(FString roomName, bool bIsPrivate);

	// 세션 호스트 이름
	FString mySessionName = "test";
	
	// 세션 생성 완료 시 호출되는 콜백 함수
	// 생성된 세션 이름, 세션 생성 성공 여부
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 방검색
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	
	void FindOtherSession();

	void OnFindSessionComplete(bool bWasSuccessful);

	// 방찾기 완료 콜백을 등록할 델리게이트
	FSearchSignature onSearchCompleted;
	// 방찾기상태 콜백 델리게이트
	FSearchStateSignature onSearchState;

	//세션방 입장
	void JoinSelectedSession(int32& index);
	void JoinPrivateRoom(FString& Code);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
};
