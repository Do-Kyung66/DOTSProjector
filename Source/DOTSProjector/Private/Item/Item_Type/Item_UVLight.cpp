// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_UVLight.h"
#include "Function_UVLight.h"

AItem_UVLight::AItem_UVLight()
{
	PrimaryActorTick.bCanEverTick = true;

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("UVLight"));
	SpotLightComp->SetupAttachment(MeshComp);
	SpotLightComp->SetRelativeLocationAndRotation(FVector(-40.f, 0.f, 0.f), FRotator(0.f, 0.f, 180.f));

	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("UVLight", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
}

void AItem_UVLight::BeginPlay()
{
	Super::BeginPlay();

	SpotLightComp->SetVisibility(false);
	ItemStrategy = NewObject<UFunction_UVLight>(this);
}

void AItem_UVLight::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if (UFunction_UVLight* UVLightLogic = Cast<UFunction_UVLight>(ItemStrategy))
	{
		UVLightLogic->Tick(DeltaTime, this);
	}
}
