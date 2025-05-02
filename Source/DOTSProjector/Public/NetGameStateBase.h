// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API ANetGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = Session)
	FString RoomName;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
};
