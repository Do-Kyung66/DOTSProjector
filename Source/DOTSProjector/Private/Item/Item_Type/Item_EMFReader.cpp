// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_EMFReader.h"
#include "GhostBase.h"
#include "Function_EMFReader.h"

AItem_EMFReader::AItem_EMFReader()
{
	PrimaryActorTick.bCanEverTick = true;

	//DetectGhostSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectGhostSphere"));
	//DetectGhostSphere->InitSphereRadius(500.f);
	//DetectGhostSphere->SetupAttachment(RootComponent);

	//DetectGhostSphere->SetGenerateOverlapEvents(true);

	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("EMFReader", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
}

void AItem_EMFReader::BeginPlay()
{
	Super::BeginPlay();
	ItemStrategy = NewObject<UFunction_EMFReader>(this);
	//DetectGhostSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_EMFReader::GhostInRange);
}

void AItem_EMFReader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UFunction_EMFReader* EMFLogic = Cast<UFunction_EMFReader>(ItemStrategy))
	{
		EMFLogic->Tick(DeltaTime, this);
	}
}


//void AItem_EMFReader::GhostInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (AGhostBase* Ghost = Cast<AGhostBase>(OtherActor))
//	{
//
//	}
//}
