// Fill out your copyright notice in the Description page of Project Settings.


#include "Evidence_Ghost.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEvidence_Ghost::AEvidence_Ghost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AEvidence_Ghost::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->SetVisibility(false);
	bIsVisible = MeshComp->IsVisible();

	if (MeshComp)
	{
		UMaterialInterface* BaseMaterial = MeshComp->GetMaterial(0);
		if (BaseMaterial)
		{
			DynamicMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			MeshComp->SetMaterial(0, DynamicMat);
		}
	}
}

// Called every frame
void AEvidence_Ghost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RevealWithUV();
	InvisibleMaterial();
}

void AEvidence_Ghost::RevealWithUV()
{
	if (DynamicMat && bIsVisible)
	{
		MeshComp->SetVisibility(bIsVisible);
		GlowIntensity = 1.0f;
		DynamicMat->SetScalarParameterValue("GlowIntensity", GlowIntensity);
	}
}

void AEvidence_Ghost::InvisibleMaterial()
{
	if (DynamicMat && !bIsVisible)
	{
		MeshComp->SetVisibility(bIsVisible);
		GlowIntensity = 0.0f;
		DynamicMat->SetScalarParameterValue("GlowIntensity", GlowIntensity);
	}
}

void AEvidence_Ghost::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEvidence_Ghost, bIsVisible);
	DOREPLIFETIME(AEvidence_Ghost, DynamicMat);
	DOREPLIFETIME(AEvidence_Ghost, GlowIntensity);
}
