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

// 게임 인스턴스에서 사용자 이름을 가져와서 Server에게 알려준다.
void ANetPlayerState::ServerRPC_SetUserName_Implementation(const FString& name)
{
	SetPlayerName(name);
	UE_LOG(LogTemp, Warning, TEXT("SetPlayerName called! name: %s"), *name);
}
