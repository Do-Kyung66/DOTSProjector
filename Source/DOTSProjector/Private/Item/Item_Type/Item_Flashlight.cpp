// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Flashlight.h"
#include "Function_Flashlight.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItem_Flashlight::AItem_Flashlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComp"));
	SpotLightComp->SetupAttachment(MeshComp);
	SpotLightComp->SetRelativeLocationAndRotation(FVector(-40.f, 0.f, 0.f), FRotator(0.f, 0.f, 180.f));

	static const FString ContextString(TEXT("ItemDataTable"));

	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("Flashlight", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
}

void AItem_Flashlight::BeginPlay()
{
	Super::BeginPlay();

	SpotLightComp->SetVisibility(false);
	ItemStrategy = NewObject<UFunction_Flashlight>(this);
}

void AItem_Flashlight::OnRep_LightState()
{
	if (SpotLightComp)
	{
		SpotLightComp->SetVisibility(bIsOn);
	}
}

void AItem_Flashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem_Flashlight, bIsOn);
}

