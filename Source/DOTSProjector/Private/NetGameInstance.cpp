// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DOTSProjector.h"

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

		// ���Ƿ� ���� ����
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
