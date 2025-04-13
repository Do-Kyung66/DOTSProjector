// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Base.h"
#include "Components/StaticMeshComponent.h"
#include "DT_Item.h"
#include "PhasmophobiaPlayerController.h"


// Sets default values
AItem_Base::AItem_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UsageStrategy = nullptr;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	ConstructorHelpers::FObjectFinder<UDataTable> TempDT(
		TEXT("/Script/Engine.DataTable'/Game/UP/Item/DT_ItemData.DT_ItemData'"));

	if (TempDT.Succeeded()) {
		ItemDataTable = TempDT.Object;
		RowNames = ItemDataTable->GetRowNames();
	}
}

// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_Base::SetUsageStrategy(UItemStrategy* NewStrategy)
{
	UsageStrategy = NewStrategy;
}

void AItem_Base::UseItem()
{
	if (UsageStrategy)
	{
		UsageStrategy->Use(this);
	}
}

void AItem_Base::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();

	APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cursor!"));
		PC->SetCursorForInteraction(true, this);

	}

}

void AItem_Base::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();

	APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->SetCursorForInteraction(false, nullptr);
	}
}