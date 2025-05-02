// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostBase.h"
#include "Ghost_Spirit.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AGhost_Spirit : public AGhostBase
{
	GENERATED_BODY()
	
public:
	AGhost_Spirit();

	virtual void Tick(float DeltaTime) override;

public:
	void UpdateFSM();

	virtual void PatrolState() override;
};
