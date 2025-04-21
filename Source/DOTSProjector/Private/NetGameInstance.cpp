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
	if (!bWasSuccessful)
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
		FSessionInfo sessionInfo;
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
