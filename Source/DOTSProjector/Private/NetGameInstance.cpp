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

	// OnlineSubsystem���� �⺻ ����ý��� �ν��Ͻ��� ������
	if (auto subsys = IOnlineSubsystem::Get())
	{	
		mySessionName.Append(FString::Printf(TEXT("_%d_%d"), FMath::Rand32(), FDateTime::Now().GetMillisecond()));
		// ����ý������κ��� �����������̽� ��������
		// ���� �������̽��� ��Ƽ�÷��̾� ���� ����, ����, ���� ���� ó��
		sessionInterface = subsys->GetSessionInterface();
		
		// ���� ���� �Ϸ� �� ȣ��� ��������Ʈ(�𸮾󳻺ο��� ó����)�� �� ��ü�� �ݹ� �Լ� ���ε�
		// OnCreateSessionComplete �Լ��� ���� ���� ��� ó��
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);

		// ���� �׽�Ʈ
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
	// �����÷뿡 �Ѱ��� ���� ����
	sessionInfo.roomName = roomName;
	sessionInfo.bIsPrivate = bIsPrivate;
	// ���� ���� ����
	FOnlineSessionSettings sessionSettings;
	
	// 1. Dedicated Server ���� ���� (���ÿ��� �ϱ� ������ false)
	sessionSettings.bIsDedicated = false;

	// 2. ����(����) ��Ī�� ���� steam ��Ī�� ���� ���� (IOnlineSubsystem�� NULL�̸� ���� LAN ��ġ)
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL");
	
	// 3. ��Ī�� �¶����� ���� ������� ����
	// false�̸� �ʴ븦 ���ؼ��� ���� ����
	// ����� ���̸� ���� x , �������� ��� ���� o
	if (bIsPrivate == true)
	{
		sessionSettings.bShouldAdvertise = false;
		// �����̺� �ڵ� ����
		int32 Code = FMath::RandRange(100000, 999999);
		sessionInfo.roomCode = FString::FromInt(Code);
		PRINTLOG(TEXT("roomCode : %s"), *sessionInfo.roomCode);
		sessionSettings.Set(FName("roomCode"), sessionInfo.roomCode, EOnlineDataAdvertisementType::ViaOnlineService);
	}
	else
	{
		sessionSettings.bShouldAdvertise = true;
	}

	// 4. �¶��� ����(Presence) ���� Ȱ�� ���� (ex. ������, ������ ���, ģ�� ����ã�� �����ϰ�)
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. ���� �����߿� ���� �㰡���� ����
	sessionSettings.bAllowJoinViaPresence = false;
	sessionSettings.bAllowJoinInProgress = false;

	// 6. ���ǿ� ������ �� �ִ� ����(public) ������ �ִ� ��� ��
	// ȣ��Ʈ�� �ڵ� ������ 3�̶� ������ �ִ� 4���̼� ���� ����
	sessionSettings.NumPublicConnections = 3;

	// 7. Ŀ���� ����� ����
	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. ȣ��Ʈ ���� ����
	sessionSettings.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	// ���ǵǾ� �ִ� ��ũ�� ���
	PRINTLOG(TEXT("Create Session Start : %s"), *mySessionName);

	// ���ǻ��� ��û ������
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
	// ���� �˻� ��ü ����
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. ���� �˻� ���� ���� (Presence: �¶��� ������ ���Ǹ� �˻�)
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	// 2. Lan ���� (NULL ����ý����̸� LAN)
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");
	// 3. �ִ� �˻� ���� ��
	sessionSearch->MaxSearchResults = 10;

	// 4. ���ǰ˻� (���� �÷��̾� �ε����� 0)
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	// ã�� ���н�
	if (!bWasSuccessful || !sessionSearch.IsValid())
	{
		onSearchState.Broadcast(false);
		PRINTLOG(TEXT("Session search failed..."));
		return;
	}
	
	// ���� �˻���� �迭
	auto results = sessionSearch->SearchResults;
	PRINTLOG(TEXT("Search Result Count : %d"), results.Num());

	// �˻��� �� ���� ������ ó��
	for (int i = 0; i < results.Num(); ++i)
	{
		auto sr = results[i];

		// ������ ��ȿ���� üũ
		if(!sr.IsValid()) continue;

		// �������� ����ü ����
		//FSessionInfo sessionInfo;
		sessionInfo.index = i;
		
		// ���� �������� �� �̸��� ȣ��Ʈ �̸� ����
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), sessionInfo.roomName);
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), sessionInfo.hostName);

		// ���� ����(����) �̸�
		FString userName = sr.Session.OwningUserName;
		// ���� ������ �÷��̾� �� (�� �ִ� �ο�)
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		// ���� ������ �÷��̾� �� (�ִ� - ���� ���� ������ ��)
		// NumOpenPublicConnetions ���������� ���������� ���� ���´�. 
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;
		sessionInfo.playerCount = FString::Printf(TEXT("%d/%d"), currentPlayerCount, maxPlayerCount);

		// ��(ms) ����
		int32 pingSpeed = sr.PingInMs;

		// ���� ���� ���
		PRINTLOG(TEXT("%s"), *sessionInfo.ToString());

		// ���� �˻��� ������ ��������Ʈ ȣ�� (�̺�Ʈ ȣ�� ���� ���� �ѱ�)
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
	// �������� ����Ʈ �Է°� �޾Ƽ� �� �� ���� ����
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

		sessionInterface->GetResolvedConnectString(sessionName, url); // ������ �̹� ����� �ʿ� �ڵ� ����

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
