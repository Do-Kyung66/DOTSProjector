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
#include "GhostBase.generated.h"


UENUM()
enum class GhostState : uint8 { 
	Idle UMETA(DisplayName = "�⺻"),
	Walking UMETA(DisplayName = "�ȱ�"),
	Chase UMETA(DisplayName = "�i��"),
	Teleport UMETA(DisplayName = "�ڷ���Ʈ"),
	Kill UMETA(DisplayName = "���̱�"),
	TriggerObject UMETA(DisplayName = "Ű��"),
	Throw UMETA(DisplayName = "������")
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

	//class ADOTSProjectorCharacter* PlayerCharacter;

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

	virtual void UpdateFSM() {};
	virtual void IdleState();
	virtual void WalkState();
	virtual void ChaseState();
	virtual void TeleportState();
	virtual void KillState();
	virtual void TriggerObjectState();
	virtual void ThrowState();

public:
	virtual void PlayerSanityChanged(float NewSanity) override;
};
