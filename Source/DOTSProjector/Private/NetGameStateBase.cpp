// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ANetGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANetGameStateBase, RoomName);
}
