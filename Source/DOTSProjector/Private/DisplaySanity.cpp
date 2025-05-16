// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplaySanity.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADisplaySanity::ADisplaySanity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	RootComponent = WidgetComponent;
}

// Called when the game starts or when spawned
void ADisplaySanity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADisplaySanity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

