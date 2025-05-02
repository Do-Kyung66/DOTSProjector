// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeButton.h"

// Sets default values
AEscapeButton::AEscapeButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEscapeButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

