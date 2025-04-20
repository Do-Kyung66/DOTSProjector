// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Item_Base.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

// Sets default values
AGhostBase::AGhostBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectItemSphere"));
	DetectItemSphere->InitSphereRadius(400.f);
	DetectItemSphere->SetupAttachment(RootComponent);
	DetectItemSphere->SetGenerateOverlapEvents(true);

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

	if (!HasAuthority()) return;

	DetectItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AGhostBase::ItemInRange);

	IdleStrategy = NewObject<UBehavior_Idle>(this);
	WalkingStrategy = NewObject<UBehavior_Walking>(this);
	ChaseStrategy = NewObject<UBehavior_Chase>(this);
	TeleportStrategy = NewObject<UBehavior_Teleport>(this);
	KillStrategy = NewObject<UBehavior_Kill>(this);
	TriggerObjectStrategy = NewObject<UBehavior_TriggerObject>(this);
	ThrowStrategy = NewObject<UBehavior_Throw>(this);
	PatrolStrategy = NewObject<UBehavior_Patrol>(this);

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

	if (GetRandomPositionInNavMesh(GetActorLocation(), 800.f, RandomPos))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->MoveToLocation(RandomPos);
		}
	}
}

// Called every frame
void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ThrowDelay += DeltaTime;

	if (ThrowDelay > 15) {
		CanThrow = true;
	}
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
	SetBehaviorStrategy(KillStrategy);
	ExecuteBehavior(&BehaviorContext);
}

void AGhostBase::TriggerObjectState()
{
	SetBehaviorStrategy(TriggerObjectStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::ThrowState()	
{
	SetBehaviorStrategy(ThrowStrategy);
	ExecuteBehavior(&BehaviorContext);
}

void AGhostBase::PatrolState()
{
	SetBehaviorStrategy(PatrolStrategy);
	ExecuteBehavior(&BehaviorContext);;
}

void AGhostBase::PlayerSanityChanged(float NewSanity)
{
	PlayerSanity = NewSanity;
	HuntBegin = true;
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

void AGhostBase::ItemInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanThrow) return;

	if (AItem_Base* Item = Cast<AItem_Base>(OtherActor)) {
		if (Item != PlayerCharacter->currentItem) {
			BehaviorContext.Item = Item;
			currentState = GhostState::Throw;
		}
	}
}

void AGhostBase::SelectTargetPlayer() 
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		AGameStateBase* GS = World->GetGameState();

		int32 RandomTargetBehavior = FMath::RandRange(0, 2);

		TArray<APhasmophobiaPlayer*> NumberOfPlayers;

		for (APlayerState* PS : GS->PlayerArray)
		{
			APhasmophobiaPlayer* NewPlayer = Cast<APhasmophobiaPlayer>(PS->GetPawn());
			if (NewPlayer)
			{
				NumberOfPlayers.Add(NewPlayer);

				if (RandomTargetBehavior == 0) {
					//GEngine->AddOnScreenDebugMessage(15, 2.0f, FColor::Cyan, TEXT("Based on Distance Target!"));
					float NearDist = FVector::DistSquared(NewPlayer->GetActorLocation(), GetActorLocation());
					if (NearDist < NearestDist)
					{
						NearestDist = NearDist;
						PlayerCharacter = NewPlayer;
					}
				}
				if (RandomTargetBehavior == 1)
				{
					//GEngine->AddOnScreenDebugMessage(16, 2.0f, FColor::Cyan, TEXT("Based on Sanity Target!"));
					if (MinSanity > NewPlayer->Sanity) {
						MinSanity = NewPlayer->Sanity;
						PlayerCharacter = NewPlayer;
					}
				}
			}
		}

		if (RandomTargetBehavior == 2)
		{
			//GEngine->AddOnScreenDebugMessage(17, 2.0f, FColor::Cyan, TEXT("Random Target!"));
			int32 RandomIndex = FMath::RandRange(0, NumberOfPlayers.Num() - 1);
			PlayerCharacter = NumberOfPlayers[RandomIndex];
		}

		if (PlayerCharacter)
		{
			BehaviorContext.Target = PlayerCharacter;
			FString PlayerName = PlayerCharacter->GetPlayerState()->GetPlayerName();
			//GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::White, FString::Printf(TEXT("Ghost Target : %s"), *PlayerName));
		}
	}
}

bool AGhostBase::GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Destination)
{
	auto NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation Loc;

	bool result = NavSys->GetRandomReachablePointInRadius(CenterLocation, Radius, Loc);
	Destination = Loc.Location;

	return result;
}