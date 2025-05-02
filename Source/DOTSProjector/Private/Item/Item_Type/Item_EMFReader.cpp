// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_EMFReader.h"
#include "GhostBase.h"
#include "Function_EMFReader.h"
#include "Components/PointLightComponent.h"

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

	LEDComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEDComp"));
	LEDComp->SetupAttachment(MeshComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> LEDTemp(TEXT("/Script/Engine.StaticMesh'/Game/Fab/EMF_Reader__Game_Ready_/LED.LED'"));
	if (LEDTemp.Succeeded())
	{
		LEDComp->SetStaticMesh(LEDTemp.Object);
	}

	
	PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
	PointLight1->SetupAttachment(LEDComp);
	PointLight1->Intensity = 100.0f;
	PointLight1->SetLightColor(FLinearColor(0.0f, 1.0f, 0.0f));
	PointLight1->SetRelativeLocation(FVector(-21.383184, -78.428730, 3.802948));
	PointLight1->SetVisibility(false);

	PointLight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight2"));
	PointLight2->SetupAttachment(LEDComp);
	PointLight2->Intensity = 100.0f;
	PointLight2->SetLightColor(FLinearColor(0.5f, 1.0f, 0.0f));
	PointLight2->SetRelativeLocation(FVector(-10.662894, -79.033676, 3.802948));
	PointLight2->SetVisibility(false);
	
	PointLight3 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight3"));
	PointLight3->SetupAttachment(LEDComp);
	PointLight3->Intensity = 100.0f;
	PointLight3->SetLightColor(FLinearColor(1.0f, 1.0f, 0.0f));
	PointLight3->SetRelativeLocation(FVector(0.090977, -80.524366, 3.802948));
	PointLight3->SetVisibility(false);

	PointLight4 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight4"));
	PointLight4->SetupAttachment(LEDComp);
	PointLight4->Intensity = 100.0f;
	PointLight4->SetLightColor(FLinearColor(1.0f, 0.5f, 0.0f));
	PointLight4->SetRelativeLocation(FVector(11.106720, -79.518648, 3.802948));
	PointLight4->SetVisibility(false);

	PointLight5 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight5"));
	PointLight5->SetupAttachment(LEDComp);
	PointLight5->Intensity = 100.0f;
	PointLight5->SetLightColor(FLinearColor(1.0f, 0.0f, 0.0f));
	PointLight5->SetRelativeLocation(FVector(21.805677, -78.341611, 3.802948));
	PointLight5->SetVisibility(false);
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
