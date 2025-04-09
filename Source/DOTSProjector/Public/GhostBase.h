// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IGhostBehaviorStrategy.h"
#include "DT_Ghost.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GhostBase.generated.h"

UCLASS()
class DOTSPROJECTOR_API AGhostBase : public ACharacter
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
	class UStaticMeshComponent* MeshComp;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UNavigationInvokerComponent* NavInvokeComp;*/


public:

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

	FGhostBehaviorData* GetRandomBehaviorEntry(const FGhostData* GhostData);

	FGhostData* GhostData;

	//class ADOTSProjectorCharacter* PlayerCharacter;
};
