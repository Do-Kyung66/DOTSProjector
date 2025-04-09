// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost_Wraith.h"

AGhost_Wraith::AGhost_Wraith()
{
	GhostData = GhostDataTable->FindRow<FGhostData>("Wraith", TEXT("GhostDataTable"));
	MeshComp->SetStaticMesh(GhostData->GhostMesh);

}
