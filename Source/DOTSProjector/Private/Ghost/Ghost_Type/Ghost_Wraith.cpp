// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Wraith.h"
#include "Behavior_Chase.h"

AGhost_Wraith::AGhost_Wraith()
{
	GhostData = GhostDataTable->FindRow<FGhostData>("Wraith", TEXT("GhostDataTable"));
	GetMesh()->SetSkeletalMesh(GhostData->GhostMesh);
	GetMesh()->SetCanEverAffectNavigation(true);
	GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeed();
}

void AGhost_Wraith::BeginPlay()
{
	Super::BeginPlay();

}

void AGhost_Wraith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateFSM();
}

void AGhost_Wraith::UpdateFSM()
{
	FString logMsg = UEnum::GetValueAsString(currentState);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, logMsg);

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
	}
}

void AGhost_Wraith::IdleState()
{
	Super::IdleState();

	int32 RandomBehavior = FMath::RandRange(0, 10);

	if (RandomBehavior < 5) {
		currentState = GhostState::Walking;
	}
	else {
		currentState = GhostState::Chase;
	}
	
}

void AGhost_Wraith::WalkState()
{
	Super::WalkState();

	if ((PlayerCharacter->GetActorLocation() - this->GetActorLocation()).Size() <= GetAttackRange())
	{
		currentState = GhostState::Teleport;
	}
}

void AGhost_Wraith::ChaseState()
{
	Super::ChaseState();

	StartGhostVisibleEvent();
	if ((PlayerCharacter->GetActorLocation() - this->GetActorLocation()).Size() <= GetAttackRange())
	{
		currentState = GhostState::Kill;
	}
	currentState = GhostState::Teleport;
}

void AGhost_Wraith::TeleportState()
{
	Super::TeleportState();

	StartGhostVisibleEvent();
	SetActorLocation(GetActorLocation() + FVector(300.f, 0.0f, 0.0f));
	currentState = GhostState::Idle;

}

void AGhost_Wraith::KillState()
{
	Super::KillState();
}

void AGhost_Wraith::TriggerObjectState()
{
	Super::TriggerObjectState();
}

void AGhost_Wraith::ThrowState()
{
	Super::ThrowState();

	GhostState::Idle;
}


