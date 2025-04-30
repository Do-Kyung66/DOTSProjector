// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostBase.h"
#include "Ghost_Wraith.generated.h"

UCLASS()
class DOTSPROJECTOR_API AGhost_Wraith : public AGhostBase
{
	GENERATED_BODY()

public:
	AGhost_Wraith();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override; 

	virtual void UpdateFSM() override;

	virtual void WalkState() override;
	virtual void ChaseState() override;
	virtual void TeleportState() override;
	virtual void IdleState() override;
	virtual void KillState() override;
	virtual void TriggerObjectState() override;
	virtual void ThrowState() override;
	virtual void PatrolState() override;
}; 
