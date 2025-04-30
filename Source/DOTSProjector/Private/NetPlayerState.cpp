// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"
#include "NetGameInstance.h"

void ANetPlayerState::BeginPlay()
{
	Super::BeginPlay();

	/*if (GetPlayerController() && GetPlayerController()->IsLocalController())
	{
		auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
		ServerRPC_SetUserName(gi->mySessionName);
	}*/

	if (HasAuthority())
	{
		auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
		if (gi)
		{
			UE_LOG(LogTemp, Warning, TEXT("mySessionName: %s"), *gi->mySessionName);
			ServerRPC_SetUserName(gi->mySessionName);

			SetPlayerName(TEXT("Player 1"));
		}
	}
}

// ���� �ν��Ͻ����� ����� �̸��� �����ͼ� Server���� �˷��ش�.
void ANetPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
{
	SetPlayerName(name);
	UE_LOG(LogTemp, Warning, TEXT("SetPlayerName called! name: %s"), *name);
}
