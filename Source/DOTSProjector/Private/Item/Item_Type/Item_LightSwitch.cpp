// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_LightSwitch.h"
#include "Function_LightSwitch.h"

AItem_LightSwitch::AItem_LightSwitch()
{
	
	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	SpotLightComp->SetupAttachment(RootComponent);
	SpotLightComp->SetRelativeLocationAndRotation(FVector(-40.f, 0.f, 0.f), FRotator(0.f, 0.f, 180.f));

	static const FString ContextString(TEXT("ItemDataTable"));

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("Flashlight", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
	MeshComp->SetupAttachment(RootComponent);

	bCanGhostTrigger = true;
}

void AItem_LightSwitch::BeginPlay()
{
	SpotLightComp->SetVisibility(false);
	ItemStrategy = NewObject<UFunction_LightSwitch>(this);
}

