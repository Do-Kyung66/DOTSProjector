// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DOTSProjectorCharacter.h"
#include "Behavior_Walking.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationInvokerComponent.h"

// Sets default values
AGhostBase::AGhostBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UDataTable> TempDT(
		TEXT("/Script/Engine.DataTable'/Game/UP/Ghost/DT_GhostData.DT_GhostData'"));

	if (TempDT.Succeeded()) {
		GhostDataTable = TempDT.Object;
		RowNames = GhostDataTable->GetRowNames();
	}

	//NavInvokeComp = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvokerComponent"));
}

// Called when the game starts or when spawned
void AGhostBase::BeginPlay()
{
	Super::BeginPlay();

	BehaviorContext.Ghost = this;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerPawn);

	if (PlayerCharacter)
	{
		BehaviorContext.Target = PlayerCharacter;
	}

	FName RandomRowName = GetRandomGhost();

	if (RandomRowName != NAME_None)
	{
		static const FString ContextString(TEXT("GhostDataTable"));
		GhostData = GhostDataTable->FindRow<FGhostData>(RandomRowName, ContextString);
		MeshComp->SetStaticMesh(GhostData->GhostMesh);
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

	ExecuteBehavior(&BehaviorContext);

}

// Called to bind functionality to input
void AGhostBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGhostBase::SetBehaviorStrategy(UGhostBehaviorStrategy* NewStrategy)
{
	CurrentStrategy = NewStrategy;
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

//FGhostBehaviorData* AGhostBase::GetRandomBehaviorEntry(const FGhostData* GhostData)
//{
//	
//}

