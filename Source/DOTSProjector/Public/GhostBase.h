// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IGhostBehaviorStrategy.h"
#include "DT_Ghost.h"
#include "GhostBase.generated.h"

UCLASS()
class DOTSPROJECTOR_API AGhostBase : public APawn
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

public:

	UGhostBehaviorStrategy* CurrentStrategy;

	void SetBehaviorStrategy(UGhostBehaviorStrategy* NewStrategy);

	void ExecuteBehavior(const FGhostBehaviorContext* Context);

	FGhostBehaviorContext BehaviorContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	UDataTable* GhostDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	FName GhostID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FName> RowNames;

	FName GetRandomGhost();

	FGhostBehaviorData* GetRandomBehaviorEntry(const FGhostData* GhostData);

	//class ADOTSProjectorCharacter* PlayerCharacter;
};
