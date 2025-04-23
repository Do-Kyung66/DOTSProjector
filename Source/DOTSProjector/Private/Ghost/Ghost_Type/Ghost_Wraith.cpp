// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Wraith.h"

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
	currentState = GhostState::Patrol;
	SetActorHiddenInGame(true);
}

void AGhost_Wraith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateFSM();

	if (!CanThrow) {
		BehaviorTimer.ThrowTimerStart += GetWorld()->GetDeltaSeconds();
		if (BehaviorTimer.ThrowTimerStart > BehaviorTimer.ThrowTimerEnd) {
			BehaviorTimer.ThrowTimerStart = 0.f;
			CanThrow = true;
		}
	}
	if (!CanTrigger) {
		BehaviorTimer.TriggerObjectTimerStart += GetWorld()->GetDeltaSeconds();
		if (BehaviorTimer.TriggerObjectTimerStart > BehaviorTimer.TriggerObjectTimerEnd) {
			BehaviorTimer.TriggerObjectTimerStart = 0.f;
			CanTrigger = true;
		}
	}
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
		case GhostState::Patrol:
			PatrolState();
			break;
	}
}

void AGhost_Wraith::IdleState()
{
	Super::IdleState();

	BehaviorTimer.IdleTimerStart += GetWorld()->GetDeltaSeconds();
	if (BehaviorTimer.IdleTimerStart > BehaviorTimer.IdleTimerEnd) {
		BehaviorTimer.IdleTimerStart = 0.f;
		SelectTargetPlayer();
		int32 RandomBehavior = FMath::RandRange(0, 10);

		if (RandomBehavior < 3) {
			currentState = GhostState::Walking;
		}
		else if (RandomBehavior < 6) {
			currentState = GhostState::Chase;
		}
		else {
			currentState = GhostState::Patrol;
		}
	}
}

void AGhost_Wraith::WalkState()
{
	Super::WalkState();

	if (PlayerCharacter) {
		if ((PlayerCharacter->GetActorLocation() - this->GetActorLocation()).Size() <= GetAttackRange())
		{
			currentState = GhostState::Kill;
			currentState = GhostState::Teleport;
		}
	}

	BehaviorTimer.WalkTimerStart += GetWorld()->GetDeltaSeconds();
	if (BehaviorTimer.WalkTimerStart > BehaviorTimer.WalkTimerEnd) {
		BehaviorTimer.WalkTimerStart = 0.f;
		int32 RandomBehavior = FMath::RandRange(0, 2);
		if (RandomBehavior == 0) {
			currentState = GhostState::Chase;
		}
		else {
			currentState = GhostState::Patrol;
		}
	}
}

void AGhost_Wraith::ChaseState()
{
	Super::ChaseState();

	VisibleRateEvent();

	if (PlayerCharacter) {
		if ((PlayerCharacter->GetActorLocation() - this->GetActorLocation()).Size() <= GetAttackRange())
		{
			currentState = GhostState::Kill;
		}
	}

	BehaviorTimer.ChaseTimerStart += GetWorld()->GetDeltaSeconds();
	if (BehaviorTimer.ChaseTimerStart > BehaviorTimer.ChaseTimerEnd) {
		BehaviorTimer.ChaseTimerStart = 0.f;
		currentState = GhostState::Teleport;
	}
}

void AGhost_Wraith::TeleportState()
{
	Super::TeleportState();

	VisibleRateEvent();
	SetActorLocation(GetActorLocation() + FVector(300.f, 0.0f, 0.0f));
	currentState = GhostState::Idle;
}

void AGhost_Wraith::KillState()
{
	if (HuntBegin) {
		SetBehaviorStrategy(KillStrategy);
		ExecuteBehavior(&BehaviorContext);
	}

	// currentState = GhostState::Idle;
}

void AGhost_Wraith::TriggerObjectState()
{
	if (CanTrigger) {
		SetBehaviorStrategy(TriggerObjectStrategy);
		ExecuteBehavior(&BehaviorContext);;
	}
	else {
		currentState = GhostState::Idle;
	}
}

void AGhost_Wraith::ThrowState()
{
	if (CanThrow && HuntBegin) {
		SetBehaviorStrategy(ThrowStrategy);
		ExecuteBehavior(&BehaviorContext);
	}
	else {
		currentState = GhostState::Idle;
	}
}

void AGhost_Wraith::PatrolState()
{
	Super::PatrolState();

	BehaviorTimer.PatrolTimerStart += GetWorld()->GetDeltaSeconds();
	if (BehaviorTimer.PatrolTimerStart > BehaviorTimer.PatrolTimerEnd) {
		BehaviorTimer.PatrolTimerStart = 0.f;
		currentState = GhostState::Idle;
		bPatrol = false;
		VisibleRateEvent();
	}
}
