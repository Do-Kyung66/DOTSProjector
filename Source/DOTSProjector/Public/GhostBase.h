// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGhostBehaviorStrategy.h"
#include "DT_Ghost.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "PhasmophobiaPlayer.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Observer.h"
#include "Behavior_Walking.h"
#include "Behavior_Chase.h"
#include "Behavior_Teleport.h"
#include "Behavior_Kill.h"
#include "Behavior_TriggerObject.h"
#include "Behavior_Throw.h"
#include "Behavior_Idle.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationInvokerComponent.h"
#include "NavigationSystem.h"
#include "Behavior_Patrol.h"
#include "GhostBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGhostVisibleEvent);

UENUM()
enum class GhostState : uint8 { 
	Idle UMETA(DisplayName = "기본"),
	Walking UMETA(DisplayName = "걷기"),
	Chase UMETA(DisplayName = "쫒기"),
	Teleport UMETA(DisplayName = "텔레포트"),
	Kill UMETA(DisplayName = "죽이기"),
	TriggerObject UMETA(DisplayName = "키기"),
	Throw UMETA(DisplayName = "던지기"),
	Patrol UMETA(DisplayName = "패트롤")
};


struct StructBehaviorTimer
{
	float IdleTimerStart = 0.f;
	float IdleTimerEnd = 2.f;

	float WalkTimerStart = 0.f;
	float WalkTimerEnd = 3.f;

	float ChaseTimerStart = 0.f;
	float ChaseTimerEnd = 2.f;

	float TeleportTimerStart = 0.f;
	float TeleportTimerEnd = 5.f;

	float KillTimerStart = 0.f;
	float KillTimerEnd = 20.f;

	float PatrolTimerStart = 0.f;
	float PatrolTimerEnd = 5.f;

	float TriggerObjectTimerStart = 0.f;
	float TriggerObjectTimerEnd = 10.f;

	float ThrowTimerStart = 0.f;
	float ThrowTimerEnd = 10.f;
};

UCLASS()
class DOTSPROJECTOR_API AGhostBase : public ACharacter, public IObserver
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGhostBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// virtual void OnConstruction(const FTransform& Transform) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UNavigationInvokerComponent* NavInvokeComp;

public:

	APhasmophobiaPlayer* PlayerCharacter;

	UPROPERTY()
	UGhostBehaviorStrategy* CurrentStrategy;

	void SetBehaviorStrategy(UGhostBehaviorStrategy* NewStrategy);

	void ExecuteBehavior(const FGhostBehaviorContext* Context);

	FGhostBehaviorContext BehaviorContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GhostData")
	UDataTable* GhostDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GhostData")
	FName GhostID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GhostData")
	TArray<FName> RowNames;

	FName GetRandomGhost();

	FGhostData* GhostData;


// List of Strategy
public:
	UPROPERTY()
	class UBehavior_Walking* WalkingStrategy;

	UPROPERTY()
	class UBehavior_Chase* ChaseStrategy;

	UPROPERTY()
	class UBehavior_Teleport* TeleportStrategy;

	UPROPERTY()
	class UBehavior_Kill* KillStrategy;

	UPROPERTY()
	class UBehavior_TriggerObject* TriggerObjectStrategy;

	UPROPERTY()
	class UBehavior_Throw* ThrowStrategy;

	UPROPERTY()
	class UBehavior_Idle* IdleStrategy;

	UPROPERTY()
	class UBehavior_Patrol* PatrolStrategy;

// Ghost Basic Ability
public:
	void StartGhostVisibleEvent();
	void ToggleVisible();
	void EndGhostVisibleEvent();

	FTimerHandle VisibleTimerHandle;
	FTimerHandle EndVisibleTimerHandle;

	bool bIsVisible;

	UPROPERTY()
	FGhostVisibleEvent VisibleMode;

	
// Behavior FSM Function
public:
	StructBehaviorTimer BehaviorTimer;

	virtual void UpdateFSM() {};
	virtual void IdleState();
	virtual void WalkState();
	virtual void ChaseState();
	virtual void TeleportState();
	virtual void KillState();
	virtual void TriggerObjectState();
	virtual void ThrowState();
	virtual void PatrolState();

// Observer
public:
	virtual void PlayerSanityChanged(float NewSanity) override;


// Ghost Stat
public:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* DetectItemSphere;

	UPROPERTY(VisibleAnywhere) 
	GhostState currentState = GhostState::Idle;

	float GetAttackRange();
	float GetMovementSpeed();
	float GetSanityDestoryRate();

	bool CanThrow = true;
	float ThrowDelay = 0.f;

	FVector RandomPos;

	bool GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Destination);

// Ghost Collision
public:
	UFUNCTION()
	void ItemInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& SweepResult);

public:
	float PlayerSanity = 0;
	float HuntBegin = false;


// Network Data
public:
	void SelectTargetPlayer();

	float NearestDist = 1000.f;
	float MinSanity = 1000.f;
};
