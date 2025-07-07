// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Spirit.h"

AGhost_Spirit::AGhost_Spirit()
{	
	GhostData = GhostDataTable->FindRow<FGhostData>("Spirit", TEXT("GhostDataTable"));
	GetMesh()->SetSkeletalMesh(GhostData->GhostMesh);
	GetMesh()->SetCanEverAffectNavigation(true);
	currentState = GhostState::Patrol;
}

void AGhost_Spirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFSM();
}

void AGhost_Spirit::UpdateFSM()
{
	FString logMsg = UEnum::GetValueAsString(currentState);
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, logMsg);

	switch (currentState) {
	case GhostState::Idle:
		IdleState();
		break;
	case GhostState::Walking:
		WalkState();
		break;
	case GhostState::Chase:
		ChaseState();
		break;
	case GhostState::Teleport:
		TeleportState();
		break;
	case GhostState::Kill:
		KillState();
		break;
	case GhostState::TriggerObject:
		TriggerObjectState();
		break;
	case GhostState::Throw:
		ThrowState();
		break;
	case GhostState::Patrol:
		PatrolState();
		break;
	}
}

void AGhost_Spirit::PatrolState()
{
	Super::PatrolState();

}

