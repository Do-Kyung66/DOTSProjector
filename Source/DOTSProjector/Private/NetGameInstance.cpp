// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DOTSProjector.h"
#include "../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h"

void UNetGameInstance::Init()
{
	Super::Init();

	// OnlineSubsystem에서 기본 서브시스템 인스턴스를 가져옴
	if (auto subsys = IOnlineSubsystem::Get())
	{	
		mySessionName.Append(FString::Printf(TEXT("_%d_%d"), FMath::Rand32(), FDateTime::Now().GetMillisecond()));
		// 서브시스템으로부터 세션인터페이스 가져오기
		// 세션 인터페이스는 멀티플레이어 세션 생성, 관리, 참여 등을 처리
		sessionInterface = subsys->GetSessionInterface();
		
		// 세션 생성 완료 시 호출될 델리게이트(언리얼내부에서 처리됨)에 이 객체의 콜백 함수 바인딩
		// OnCreateSessionComplete 함수가 세션 생성 결과 처리
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);

		// 세션 테스트
		//FTimerHandle handle;
		//GetWorld()->GetTimerManager().SetTimer(handle,
		//								FTimerDelegate::CreateLambda([&]
		//								{
		//								//CreateMySession(mySessionName, false);
		//								FindOtherSession();
		//								}
		//								), 2.0f, false);
	}
}

void UNetGameInstance::CreateMySession(FString roomName, bool bIsPrivate)
{
	// FSessionInfo sessionInfo;
	// 웨이팅룸에 넘겨줄 정보 저장
	sessionInfo.roomName = roomName;
	sessionInfo.bIsPrivate = bIsPrivate;
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
		// 프라이빗 코드 생성
		int32 Code = FMath::RandRange(100000, 999999);
		sessionInfo.roomCode = FString::FromInt(Code);
		PRINTLOG(TEXT("roomCode : %s"), *sessionInfo.roomCode);
		sessionSettings.Set(FName("roomCode"), sessionInfo.roomCode, EOnlineDataAdvertisementType::ViaOnlineService);
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
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Player/Map/WaitingRoom?listen"));
	}
}

void UNetGameInstance::FindOtherSession()
{
	onSearchState.Broadcast(true);
	// 세션 검색 객체 생성
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. 세션 검색 조건 설정 (Presence: 온라인 상태의 세션만 검색)
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	// 2. Lan 여부 (NULL 서브시스템이면 LAN)
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");
	// 3. 최대 검색 세션 수
	sessionSearch->MaxSearchResults = 10;

	// 4. 세션검색 (로컬 플레이어 인덱스는 0)
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	// 찾기 실패시
	if (!bWasSuccessful || !sessionSearch.IsValid())
	{
		onSearchState.Broadcast(false);
		PRINTLOG(TEXT("Session search failed..."));
		return;
	}
	
	// 세션 검색결과 배열
	auto results = sessionSearch->SearchResults;
	PRINTLOG(TEXT("Search Result Count : %d"), results.Num());

	// 검색된 각 세션 정보를 처리
	for (int i = 0; i < results.Num(); ++i)
	{
		auto sr = results[i];

		// 정보가 유효한지 체크
		if(!sr.IsValid()) continue;

		// 세션정보 구조체 선언
		//FSessionInfo sessionInfo;
		sessionInfo.index = i;
		
		// 세션 설정에서 방 이름과 호스트 이름 추출
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), sessionInfo.roomName);
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), sessionInfo.hostName);

		// 세션 주인(방장) 이름
		FString userName = sr.Session.OwningUserName;
		// 입장 가능한 플레이어 수 (방 최대 인원)
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		// 현재 입장한 플레이어 수 (최대 - 현재 입장 가능한 수)
		// NumOpenPublicConnetions 스팀에서만 정상적으로 값이 들어온다. 
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;
		sessionInfo.playerCount = FString::Printf(TEXT("%d/%d"), currentPlayerCount, maxPlayerCount);

		// 핑(ms) 정보
		int32 pingSpeed = sr.PingInMs;

		// 세션 정보 출력
		PRINTLOG(TEXT("%s"), *sessionInfo.ToString());

		// 세션 검색이 끝나면 델리게이트 호출 (이벤트 호출 세션 정보 넘김)
		onSearchCompleted.Broadcast(sessionInfo);
	}
	onSearchState.Broadcast(false);
}

void UNetGameInstance::JoinSelectedSession(int32& index)
{	
	auto sr = sessionSearch->SearchResults;

	sr[index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	sr[index].Session.SessionSettings.bUsesPresence = true;

	sessionInterface->JoinSession(0, FName(mySessionName), sr[index]);
}

void UNetGameInstance::JoinPrivateRoom(FString& Code)
{
	//FSessionInfo sessionInfo;
	// 위젯에서 에디트 입력값 받아서 비교 후 세션 조인
	auto sr = sessionSearch->SearchResults;
	for (int32 i = 0; i < sr.Num(); i++)
	{
		FString FoundRoomCode;
		sr[i].Session.SessionSettings.Get(FName("roomCode"), FoundRoomCode);

		if (FoundRoomCode == Code)
		{
			sessionInterface->JoinSession(0, FName(mySessionName), sr[i]);
			PRINTLOG(TEXT("join! %s = %s"), *sessionInfo.roomCode, *Code);
			return;
		}
		else
		{
			PRINTLOG(TEXT("join faild roomCode : %s, Code : %s"), *sessionInfo.roomCode, *Code);
		}
	}

	
}

void UNetGameInstance::OnJoinSessionComlete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		auto pc = GetWorld()->GetFirstPlayerController();
		FString url;

		sessionInterface->GetResolvedConnectString(sessionName, url); // 서버가 이미 열어둔 맵에 자동 진입

		PRINTLOG(TEXT("Join URL : %s"), *url);

		if(!url.IsEmpty())
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		PRINTLOG(TEXT("Join Session failed : %d"), result);
	}
}
