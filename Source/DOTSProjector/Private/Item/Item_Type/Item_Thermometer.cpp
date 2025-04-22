// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Thermometer.h"
#include "Function_Thermometer.h"

AItem_Thermometer::AItem_Thermometer()
{
	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("Thermometer", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
}

void AItem_Thermometer::BeginPlay()
{
	Super::BeginPlay();
	ItemStrategy = NewObject<UFunction_Thermometer>(this);
}

void AItem_Thermometer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UFunction_Thermometer* ThermometerLogic = Cast<UFunction_Thermometer>(ItemStrategy))
	{
		ThermometerLogic->Tick(DeltaTime, this);
	}
}
