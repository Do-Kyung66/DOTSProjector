// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Behavior_Walking.h"
#include "GameFramework/Character.h"
#include "NavigationInvokerComponent.h"
#include "Behavior_Chase.h"
#include "Behavior_Teleport.h"
#include "Behavior_Kill.h"
#include "Behavior_TriggerObject.h"
#include "Behavior_Throw.h"
#include "Behavior_Idle.h"

// Sets default values
AGhostBase::AGhostBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UDataTable> TempDT(
		TEXT("/Script/Engine.DataTable'/Game/UP/Ghost/DT_GhostData.DT_GhostData'"));

	if (TempDT.Succeeded()) {
		GhostDataTable = TempDT.Object;
		RowNames = GhostDataTable->GetRowNames();
	}

	NavInvokeComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
}

// Called when the game starts or when spawned
void AGhostBase::BeginPlay()
{
	Super::BeginPlay();

	IdleStrategy = NewObject<UBehavior_Idle>(this);
	WalkingStrategy = NewObject<UBehavior_Walking>(this);
	ChaseStrategy = NewObject<UBehavior_Chase>(this);
	TeleportStrategy = NewObject<UBehavior_Teleport>(this);
	KillStrategy = NewObject<UBehavior_Kill>(this);
	TriggerObjectStrategy = NewObject<UBehavior_TriggerObject>(this);
	ThrowStrategy = NewObject<UBehavior_Throw>(this);

	BehaviorContext.Ghost = this;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerCharacter = Cast<APhasmophobiaPlayer>(PlayerPawn);

	if (PlayerCharacter)
	{	
		PlayerCharacter->AddObserver(this);
		BehaviorContext.Target = PlayerCharacter;
	}

	FName RandomRowName = GetRandomGhost();

	if (RandomRowName != NAME_None)
	{
		static const FString ContextString(TEXT("GhostDataTable"));
		GhostData = GhostDataTable->FindRow<FGhostData>(RandomRowName, ContextString);
	}

	if (GhostData && GhostData->BehaviorDatas.Num() > 0)
	{
		const FGhostBehaviorData& Info = GhostData->BehaviorDatas[0];

		if (Info.BehaviorClass)
		{
			UGhostBehaviorStrategy* Strategy = NewObject<UGhostBehaviorStrategy>(this, Info.BehaviorClass);
			SetBehaviorStrategy(Strategy);
			ExecuteBehavior(&BehaviorContext);
		}
	}
}

// Called every frame
void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//ExecuteBehavior(&BehaviorContext);


}

// Called to bind functionality to input
void AGhostBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGhostBase::SetBehaviorStrategy(UGhostBehaviorStrategy* NewStrategy)
{
	if (CurrentStrategy != NewStrategy)
	{
		CurrentStrategy = NewStrategy;
	}
}

void AGhostBase::ExecuteBehavior(const FGhostBehaviorContext* Context)
{
	if (CurrentStrategy)
	{
		CurrentStrategy->ExecuteBehavior(*Context);
	}
}

FName AGhostBase::GetRandomGhost()
{
	if (GhostDataTable)
	{
		if (RowNames.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, RowNames.Num() - 1);
			return RowNames[Index];
		}
	}

	return NAME_None;
}

void AGhostBase::StartGhostVisibleEvent()
{
	ToggleVisible();

	GetWorld()->GetTimerManager().SetTimer(
		EndVisibleTimerHandle, this, &AGhostBase::EndGhostVisibleEvent, 3.0f, false
	);
}

void AGhostBase::ToggleVisible()
{
	bIsVisible = !bIsVisible;
	SetActorHiddenInGame(!bIsVisible);

	// VisibleMode.Broadcast();

	float NextInterval = FMath::FRandRange(0.5f, 1.f);

	GetWorld()->GetTimerManager().SetTimer(
		VisibleTimerHandle, this, &AGhostBase::ToggleVisible, NextInterval, false
	);
}

void AGhostBase::EndGhostVisibleEvent()
{
	bIsVisible = false;
	SetActorHiddenInGame(true);

	GetWorld()->GetTimerManager().ClearTimer(VisibleTimerHandle);
}

void AGhostBase::IdleState()
{
	SetBehaviorStrategy(IdleStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::WalkState()
{
	SetBehaviorStrategy(WalkingStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::ChaseState()
{
	SetBehaviorStrategy(ChaseStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::TeleportState()
{
	SetBehaviorStrategy(TeleportStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::KillState()
{
	SetBehaviorStrategy(TeleportStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::TriggerObjectState()
{
	SetBehaviorStrategy(TriggerObjectStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::ThrowState()
{
	SetBehaviorStrategy(TeleportStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::PlayerSanityChanged(float NewSanity)
{

}

float AGhostBase::GetAttackRange()
{
	return GhostData->AttackRange;
}	

float AGhostBase::GetMovementSpeed()
{
	return GhostData->MovementSpeed;
}

float AGhostBase::GetSanityDestoryRate()
{
	return GhostData->SanityDestroyRate;
}


