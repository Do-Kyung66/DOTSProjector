// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayMission.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADisplayMission::ADisplayMission()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    RootComponent = WidgetComponent;

    //static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_PlayerSanityUI")); 
    //if (WidgetClass.Succeeded())
    //{
    //    WidgetComponent->SetWidgetClass(WidgetClass.Class);
    //    WidgetComponent->SetDrawSize(FVector2D(600, 400));
    //    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    //}
}

// Called when the game starts or when spawned
void ADisplayMission::BeginPlay()   
{
	Super::BeginPlay();
	
}

// Called every frame
void ADisplayMission::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

