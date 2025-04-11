// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Wraith.h"

AGhost_Wraith::AGhost_Wraith()
{
	GhostData = GhostDataTable->FindRow<FGhostData>("Wraith", TEXT("GhostDataTable"));
	MeshComp->SetStaticMesh(GhostData->GhostMesh);
	MeshComp->SetCanEverAffectNavigation(true);
}

void AGhost_Wraith::BeginPlay()
{
	Super::BeginPlay();

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

void AGhost_Wraith::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ExecuteBehavior(&BehaviorContext);
}


