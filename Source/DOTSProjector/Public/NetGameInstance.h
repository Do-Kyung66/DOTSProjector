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
	// 게임 인스턴스 초기화 함수(게임 시작 시 자동 호출됨)
	virtual void Init() override;

public:
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
	
};
