// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DOTSProjector.h"

void UNetGameInstance::Init()
{
	Super::Init();

	// OnlineSubsystem에서 기본 서브시스템 인스턴스를 가져옴
	if (auto subsys = IOnlineSubsystem::Get())
	{	
		// 서브시스템으로부터 세션인터페이스 가져오기
		// 세션 인터페이스는 멀티플레이어 세션 생성, 관리, 참여 등을 처리
		sessionInterface = subsys->GetSessionInterface();
		
		// 세션 생성 완료 시 호출될 델리게이트(언리얼내부에서 처리됨)에 이 객체의 콜백 함수 바인딩
		// OnCreateSessionComplete 함수가 세션 생성 결과 처리
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);

		// 임의로 세션 생성
		/*FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,
										FTimerDelegate::CreateLambda([&]
										{
										CreateMySession(mySessionName, false);
										}
										), 2.0f, false);*/
	}
}

void UNetGameInstance::CreateMySession(FString roomName, bool bIsPrivate)
{
	// 세션 설정 변수
	FOnlineSessionSettings sessionSettings;
	
	// 1. Dedicated Server 접속 여부 (로컬에서 하기 때문에 false)
	sessionSettings.bIsDedicated = false;

	// 2. 랜선(로컬) 매칭을 할지 steam 매칭을 할지 여부 (IOnlineSubsystem이 NULL이면 로컬 LAN 매치)
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL");
	
	// 3. 매칭이 온라인을 통해 노출될지 여부
	// false이면 초대를 통해서만 입장 가능
	// 비공개 방이면 노출 x , 공개방일 경우 노출 o
	if (bIsPrivate == true)
	{
		sessionSettings.bShouldAdvertise = false;
	}
	else
	{
		sessionSettings.bShouldAdvertise = true;
	}

	// 4. 온라인 상태(Presence) 정보 활용 여부 (ex. 접속중, 게임중 등등, 친구 세션찾기 가능하게)
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임 진행중에 참여 허가할지 여부
	sessionSettings.bAllowJoinViaPresence = false;
	sessionSettings.bAllowJoinInProgress = false;

	// 6. 세션에 참여할 수 있는 공개(public) 연결의 최대 허용 수
	// 호스트는 자동 참여라 3이라 적으면 최대 4명이서 게임 가능
	sessionSettings.NumPublicConnections = 3;

	// 7. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. 호스트 네임 설정
	sessionSettings.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	// 정의되어 있는 매크로 사용
	PRINTLOG(TEXT("Create Session Start : %s"), *mySessionName);

	// 세션생성 요청 보내기
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	PRINTLOG(TEXT("SessionName : %s, bWasSuccessful : %d"), *SessionName.ToString(), bWasSuccessful);
}
