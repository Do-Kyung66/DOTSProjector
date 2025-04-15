// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_VideoCamera.h"
#include "Function_VideoCamera.h"

AItem_VideoCamera::AItem_VideoCamera()
{
	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("VideoCamera", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
	CaptureComponent->SetupAttachment(MeshComp);

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RT(TEXT("/Game/UP/Item/ItemTool/RT_VideoCamera.RT_VideoCamera"));
	if (RT.Succeeded())
	{
		RenderTarget = RT.Object;
		CaptureComponent->TextureTarget = RenderTarget;
	}
}

void AItem_VideoCamera::BeginPlay()
{
	Super::BeginPlay();
	ItemStrategy = NewObject<UFunction_VideoCamera>(this);
}
