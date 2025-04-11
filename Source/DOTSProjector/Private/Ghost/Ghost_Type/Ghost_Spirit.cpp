// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Spirit.h"

AGhost_Spirit::AGhost_Spirit()
{	
	GhostData = GhostDataTable->FindRow<FGhostData>("Spirit", TEXT("GhostDataTable"));
	GetMesh()->SetSkeletalMesh(GhostData->GhostMesh);
	GetMesh()->SetCanEverAffectNavigation(true);
}

void AGhost_Spirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

