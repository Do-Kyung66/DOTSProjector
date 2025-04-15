// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_EMFReader.h"
#include "GhostBase.h"

AItem_EMFReader::AItem_EMFReader()
{
	DetectGhostSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectGhostSphere"));
	DetectGhostSphere->InitSphereRadius(500.f);
	DetectGhostSphere->SetupAttachment(RootComponent);


	DetectGhostSphere->SetGenerateOverlapEvents(true);
}

void AItem_EMFReader::BeginPlay()
{
	Super::BeginPlay();
	DetectGhostSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_EMFReader::GhostInRange);
}

void AItem_EMFReader::GhostInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGhostBase* Ghost = Cast<AGhostBase>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Red, TEXT("Ghost Detected!"));
	}
}
