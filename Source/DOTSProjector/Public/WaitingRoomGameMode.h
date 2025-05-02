// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WaitingRoomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AWaitingRoomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	AWaitingRoomGameMode();
public:
	virtual void BeginPlay() override;
	// 플레이어 나갈 때 자동 호출
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> WaitingRoomWidget;

	class UWaitingRoomWidget* Widget;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	void CheckRoomEmpty();
	void DestroySession();
};
